//
// Created by artem on 25.09.2024.
//

#ifndef CPP_SEMINARS_TREAP_H
#define CPP_SEMINARS_TREAP_H


#include <random>
#include <ctime>
#include <iostream>
#include <memory>
const int INF = 1e9;
//std::mt19937 rng(time(0));
//std::uniform_int_distribution<int> uid(-INF, INF);


class Pair {
public:
    int first;
    int second;
    Pair(int first, int second) : first(first), second(second){};
};
template<class K, class V>
class Treap {
public:
    struct Node {
        int prior;
        K key;
        V value;
        std::pair<K, V>* p;
        Node *l, *r;
        Node(K key, V value) : key(key), value(value) {
            prior = rand();
            l = nullptr;
            r = nullptr;

            p = new std::pair<K, V>{key, value};
        }
    };
    Node* root;
    Treap() {
        root = 0;
    }
    Node* merge(Node*, Node*);
    void split(Node* t, K x, Node*& a, Node*& b, bool flag = false);
    V find(const K &key);
    V get(Node* t, K key);
    void insert(const K &key, const V &value);
    void erase(const K &key);
    void traverse(Node* t);
    void traverse();
    Node* getBegin(Node* t);

    Node* getEnd(Node* t);


    class Iterator {
    private:
        std::vector<Node*> prev = {nullptr};

    public:
        Iterator(Node* _it) {
            goLeft(_it);
        }
        std::pair<K, V>& operator*() {
            return *(prev.back()->p);
        }

        std::pair<K, V>* operator->() {
            return prev.back()->p;
        }

        void goLeft(Node* node) {
            while(node) {
                prev.push_back(node);
                node = node->l;
            }
        }

        Iterator& operator++() {
            Node* node = prev.back();
            prev.pop_back();
            if (node->r) {
                goLeft(node->r);
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            Node* node = prev.back();
            prev.pop_back();
            if (node->r) {
                goLeft(node->r);
            }
            return temp;
        }

        bool operator!=(const Iterator& other) const {
            return prev.back() != other.prev.back();
        }

        bool operator==(const Iterator& other) const {
            return prev.back() == other.prev.back();
        }
    };
    Iterator begin();

    Iterator end();



};
template<class K, class V>
typename Treap<K, V>::Iterator Treap<K, V>::begin() {
    //return Iterator(*root);
    return Iterator(root);
    //Treap<K, V>::Iterator::prev
}
template<class K, class V>
typename Treap<K, V>::Iterator Treap<K, V>::end() {
    return Iterator(nullptr);
}

template<class K, class V>
typename Treap<K, V>::Node* Treap<K, V>::getBegin(Node* t) {
    if (!t->l) {
        return t;
    }
    return getBegin(t->l);
}

template<class K, class V>
typename Treap<K, V>::Node* Treap<K, V>::getEnd(Node* t) {
    if (!(t->r)) {
        return t;
    }
    return getEnd(t->r);
}

template<class K, class V>
typename Treap<K, V>::Node* Treap<K, V>::merge(Node* l, Node* r) {
    if (!l) {
        return r;
    }
    if  (!r) {
        return l;
    }
    if (l->prior > r->prior) {
        l->r = merge(l->r, r);
        return l;
    } else {
        r->l = merge(l, r->l);
        return r;
    }
}

template<class K, class V>
void Treap<K, V>::split(Node* t, K x, Node*& a, Node*& b, bool flag) {
    if (!t) {
        a = 0;
        b = 0;
        return;
    }
    if (!flag) {
        if (t->key < x) {
            split(t->r, x, t->r, b);
            a = t;
        }
        else {
            split(t->l, x, a, t->l);
            b = t;
        }
    } else {
        if (t->key <= x) {
            split(t->r, x, t->r, b);
            a = t;
        }
        else {
            split(t->l, x, a, t->l);
            b = t;
        }
    }

}

template<class K, class V>
V Treap<K, V>::find(const K &key) {
    int val = get(root, key);
    if (val == INF) {
        return INF;
    } else {
        return val;
    }
}

template<class K, class V>
V Treap<K, V>::get(Node* t, K key) {
    if (!t) {
        return INF;
    }
    if (t->key == key) {
        return t->value;
    }
    if (t->key < key) {
        return get(t->r, key);
    } else {
        return get(t->l, key);
    }
}

template<class K, class V>
void Treap<K, V>::insert(const K &key, const V &value) {
    int val = get(root, key);
    if (val != INF) {
        return;
    }
    Node *a, *b;
    split(root, key, a, b);
    Node* t = new Node(key, value);
    root = merge(a, merge(t, b));
}

template<class K, class V>
void Treap<K, V>::erase(const K &key) {
    Node *a, *b, *cur;
    split(root, key, a, cur);
    split(cur, key, cur, b, true);
    root = merge(a, b);
}

template<class K, class V>
void Treap<K, V>::traverse(Node* t) {
    if (!t) {
        return;
    }
    traverse(t->l);
    std::cout << t->key << ' ' << t->value << std::endl;
    traverse(t->r);
}

template<class K, class V>
void Treap<K, V>::traverse() {
    traverse(root);
}

#endif //CPP_SEMINARS_TREAP_H