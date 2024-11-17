#include "statement.h"
#include <cassert>
#include <string>
#include <iostream>
#include <chrono>

int main() {
//    clock_t start, finish;
//    start = clock();
//    std::string str;
////    for (int i = 0; i < 20000; ++i) {
////        str += "0 ";
////    }
////    str += "+ + + ";
////
//    str = "1 +";
//    auto op = compile(str);
//    std::cout << op->get_arguments_count() << ' ' << op->get_results_count();
//    std::vector<int> ans = op->apply({});
//    for (auto i : ans) {
//        std::cout << i << ' ';
//    }
//    std::cout << std::endl;
//    finish = clock();
//    double duration = (double)(finish - start) / CLOCKS_PER_SEC;
//    std::cout << duration << std::endl;
    auto plus = compile("+");
    auto minus = compile("-");
    auto inc = compile("1 +");

    assert(plus->is_pure() && plus->get_arguments_count() == 2 && plus->get_results_count() == 1);
    assert(inc->is_pure() && inc->get_arguments_count() == 1 && inc->get_results_count() == 1);

    assert(plus->apply({2, 2}) == std::vector{4});
    assert(minus->apply({1, 2, 3}) == std::vector({1, -1}));

    auto plus_4 = inc | inc | inc | inc;

    assert(plus_4->is_pure() && plus_4->get_arguments_count() == 1 && plus_4->get_results_count() == 1);
    assert(plus_4->apply({0}) == std::vector{4});
    assert(inc->apply({0}) == std::vector{1});

    auto dup = compile("dup");
    assert(dup->is_pure() && dup->get_arguments_count() == 1 && dup->get_results_count() == 2);

    auto sqr = dup | compile("*");
    auto ten = compile("6") | plus_4;
    assert((ten | sqr)->apply({}) == std::vector{100});

    auto complicated_zero = compile(" 1    4  3 4   5  6 + -      - 3    / % -    ");
    assert(complicated_zero->is_pure() && complicated_zero->get_arguments_count() == 0 && complicated_zero->get_results_count() == 1);
    assert(complicated_zero->apply({}) == std::vector{0});

    for (int i = 0; i < 1000; ++i) {
        auto i_str = std::to_string(i);
        auto plus_i = "+" + i_str;
        auto minus_i = "-" + i_str;

        int res1 = compile(i_str)->apply({})[0];
        int res2 = compile(plus_i)->apply({})[0];
        int res3 = compile(minus_i)->apply({})[0];

        assert(res1 == i);
        assert(res2 == i);
        assert(res3 == -i);
    }

    auto nop = compile("");
    assert(nop->is_pure() && nop->get_arguments_count() == 0 && nop->get_results_count() == 0);
}