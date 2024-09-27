#include "Treap.h"

int main () {
    struct Treap<int, int> treap{};
    treap.insert(1, 2);
    treap.insert(2, 4);
    treap.insert(1, 4);
    treap.insert(10, -1);
    treap.insert(0, 1);
    treap.erase(1);
    treap.erase(3);
    //treap.traverse();
    std::cout << '\n';
//    for (auto i = treap.begin(); i != treap.end(); ++i) {
//        std::cout << i->first << ' ' << i->second << std::endl;
//        std::cout << (*i).first << ' ' << (*i).second << std::endl;
//    }
//    for (const auto& [k, v] : treap) {
//        std::cout << k << ' ' << v <<std::endl;
//    }
}
