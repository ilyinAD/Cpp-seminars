//
// Created by artem on 09.11.2024.
//

#include "myvector.h"

#include <mutex>
#include <memory>
#include <new>
#include <algorithm>
#include <type_traits>

template<class T, class Allocator = std::allocator<T>>
requires(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>)
class ConcurrentVector: private Allocator {
public:
    using AllocatorTrits = std::allocator_traits<Allocator>;
    using size_type = AllocatorTrits::size_type;
    using pointer = AllocatorTrits::pointer;
    using const_pointer = AllocatorTrits::const_pointer;
private:
    pointer data = nullptr;
    size_type capacity = 0, sz = 0;
    std::mutex mutex;

    Allocator &get_allocator() {
        return *static_cast<Allocator*>(this);
    }

    void realloc() {
        pointer old_data = data;
        auto new_capacity = capacity + std::max<size_type>(capacity >> 1, 1);
        data = AllocatorTrits::allocate(get_allocator(), new_capacity);
        capacity = new_capacity;
        for (size_type i = 0; i < sz; ++i) {
            AllocatorTrits::construct(get_allocator(), data + i, std::move(old_data[i]));
            AllocatorTrits::destroy(get_allocator(), old_data + i);
        }
        AllocatorTrits::deallocate(get_allocator(), old_data, sz);
    }
public:

    ConcurrentVector() = default;
    ConcurrentVector(const ConcurrentVector&) = delete;
    ConcurrentVector &operator=(const ConcurrentVector&) = delete;

    ~ConcurrentVector() noexcept {
        for (size_type i = 0; i < sz; ++i) {
            AllocatorTrits::destroy(get_allocator(), data + i);
        }
        AllocatorTrits::deallocate(get_allocator(), data, capacity);
    }

    template<class... Args>
    requires(std::is_nothrow_constructible_v<T, Args&&...>)
    void emplace_back(Args&&... args) {
        std::lock_guard g(mutex);
        if (sz == capacity) {
            realloc();
        }
        AllocatorTrits::construct(get_allocator(), data + sz, std::forward<Args>(args)...);
        ++sz;
    }

    size_type size() const {
        return sz;
    }

    const_pointer begin() const {
        return data;
    }

    const_pointer end() const {
        return data + size();
    }
};
