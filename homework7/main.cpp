#include <atomic>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <limits>
#include <iterator>
#include <cassert>
#include <mutex>

namespace detail {

    template<bool store_hash>
    using StateType = std::conditional_t<store_hash, size_t, unsigned char>;

    template<bool store_hash>
    struct StateGen {
        enum class type: StateType<store_hash> {
            MAX_EMPLACED = std::numeric_limits<StateType<store_hash>>::max() - 2,
            EMPTY,
            REMOVED,
        };
    };

    template<bool store_hash>
    using State = StateGen<store_hash>::type;

    template<bool store_hash>
    auto operator<=>(State<store_hash> lhs, State<store_hash> rhs) {
        using U = std::underlying_type_t<State<store_hash>>;
        return (U) lhs <=> (U) rhs;
    }

    template<class T, bool store_hash>
    struct HashTableCell {
        std::atomic<State<store_hash>> state{State<store_hash>::EMPTY};
        union {
            char dummy;
            T data;
        };

        HashTableCell() {}
        ~HashTableCell() {}
    };

}

// If store_hash is true, hash of each element must be calculated only once
template<class T, bool store_hash = (sizeof(T) > sizeof(size_t)), class Hasher = std::hash<T>, class Comparator = std::equal_to<T>, class AllocBase = std::allocator<void>>
requires(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>)
class HashMultiset: private Hasher, private Comparator, private std::allocator_traits<AllocBase>::rebind_alloc<detail::HashTableCell<T, store_hash>> {
private:
    using State = detail::State<store_hash>;
    using CellType = detail::HashTableCell<T, store_hash>;
    using Allocator = typename std::allocator_traits<AllocBase>::rebind_alloc<CellType>;
    using AllocatorTraits = std::allocator_traits<Allocator>;
    using CellPtr = typename AllocatorTraits::pointer;

public:
    using value_type = T;
    using size_type = typename AllocatorTraits::size_type;
    using difference_type = typename AllocatorTraits::difference_type;

private:
    std::atomic<uint8_t> active_threads{0};
    CellPtr data;
    std::atomic<size_type> sz{0};
    std::atomic<size_type> dirty{0};
    size_type capacity = 4;
    std::atomic_flag reallocation{}; // flag for reallocation
    mutable std::mutex resize_mutex;  // Mutex to protect resizing

    template<class Fn>
    void try_cells(size_type hash, Fn &&fn) const {
        size_type pos = hash & (capacity - 1);
        State cur;
        while (!fn(data[pos])) {
            pos = (pos * 5 + 3 + hash) & (capacity - 1);
            hash >>= 5;
        }
    }

    // Ensure proper locking during resizing and reallocation
    CellPtr realloc() {
        std::lock_guard<std::mutex> lock(resize_mutex);  // Mutex to ensure single thread performs the reallocation
        if (sz.load() * 4 > capacity) // When capacity is full, expand the table
            capacity *= 2;

        auto old_data = data;
        data = AllocatorTraits::allocate(get_allocator(), capacity);
        for (size_type i = 0; i < capacity; ++i) {
            AllocatorTraits::construct(get_allocator(), data + i);
        }
        return old_data;
    }

    // Try to place the element in the appropriate location (using hashing and atomic operations)
    void emplace_hash(size_t hash, T &&elem) {
        State final_state;
        if constexpr (store_hash) {
            final_state = (State) hash;
        } else {
            final_state = State::MAX_EMPLACED;
        }

        try_cells(hash, [&, this](CellType &cell) {
            State old_state = cell.state.load();
            bool found;
            while ((found = (old_state == State::EMPTY || old_state == State::REMOVED)) && cell.state.compare_exchange_weak(old_state, final_state)) {}
            if (!found)
                return false;
            AllocatorTraits::construct(get_allocator(), &cell.data, std::move(elem));
            return true;
        });
    }

    // Calculate hash value
    size_t calc_hash(const T &x) const {
        size_t ret = get_hasher()(x);
        if constexpr (store_hash) {
            return ret <= (size_t) State::MAX_EMPLACED ? ret : (size_t) State::MAX_EMPLACED;
        } else {
            return ret;
        }
    }

    // Find a specific element in the table
    template<bool rem>
    bool find(const T &key) const {
        size_t hash = calc_hash(key);
        bool ret = false;
        State target;
        if constexpr (store_hash) {
            target = (State) hash;
        } else {
            target = State::MAX_EMPLACED;
        }
        try_cells(hash, [&](CellType &cell) {
            State old_state = cell.state.load();
            if (old_state == State::EMPTY) {
                return true;
            } else if (old_state == target) {
                if (get_comparator()(cell.data, key)) {
                    if constexpr (rem) {
                        cell.state = State::REMOVED;
                        cell.data.~T();
                        const_cast<std::atomic<size_type> &>(sz).fetch_sub(1);
                    }
                    ret = true;
                    return true;
                }
            }
            return false;
        });
        return ret;
    }

public:
    HashMultiset() {
        data = AllocatorTraits::allocate(get_allocator(), capacity);
        for (size_type i = 0; i < capacity; ++i) {
            AllocatorTraits::construct(get_allocator(), data + i);
        }
    }

    ~HashMultiset() {
        for (size_type i = 0; i < capacity; ++i) {
            if (data[i].state.load() <= State::MAX_EMPLACED) {
                data[i].data.~T();
            }
            AllocatorTraits::destroy(get_allocator(), data + i);
        }
        AllocatorTraits::deallocate(get_allocator(), data, capacity);
    }

    const Hasher &get_hasher() const {
        return *static_cast<const Hasher*>(this);
    }

    Allocator &get_allocator() {
        return *static_cast<Allocator*>(this);
    }

    const Comparator &get_comparator() const {
        return *static_cast<const Comparator*>(this);
    }

    size_type size() const {
        return sz.load();
    }

    // Emplace new element in multiset
    template<class... Args>
    requires(std::is_nothrow_constructible_v<T, Args&&...>)
    void emplace(Args&&... args) {
        while (reallocation.test())
            reallocation.wait(true);
        active_threads.fetch_add(1);
        sz.fetch_add(1);
        auto d = dirty.fetch_add(1);
        while (d * 2 >= capacity) {
            if (reallocation.test_and_set()) {
                reallocation.wait(true);
                d = dirty.load();
                continue;
            }
            auto active = active_threads.fetch_sub(1) - 1;
            while (active > 0) {
                active = active_threads.load();
            }
            size_type old_capacity = capacity;
            dirty.store(d = sz.load());
            auto old_data = realloc();
            for (size_type i = 0; i < old_capacity; ++i) {
                if (old_data[i].state.load() <= State::MAX_EMPLACED) {
                    size_t hash;
                    if constexpr (store_hash) {
                        hash = (size_t) old_data[i].state.load();
                    } else {
                        hash = calc_hash(old_data[i].data);
                    }
                    emplace_hash(hash, std::move(old_data[i].data));
                    old_data[i].data.~T();
                }
                AllocatorTraits::destroy(get_allocator(), old_data + i);
            }
            active_threads.store(1);
            reallocation.clear();
            reallocation.notify_all();
            AllocatorTraits::deallocate(get_allocator(), old_data, old_capacity);
        }
        T elem(std::forward<Args>(args)...);
        auto hash = calc_hash(elem);
        emplace_hash(hash, std::move(elem));
        active_threads.fetch_sub(1);
    }

    bool contains(const T &key) const {
        return find<false>(key);
    }

    bool erase(const T &key) const {
        return find<true>(key);
    }
};