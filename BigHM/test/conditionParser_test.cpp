//
// Created by artem on 25.11.2024.
//
#include "gtest/gtest.h"
#include "conditionParser/ExprChecker.h"
#include <map>

TEST(conditionParser, parseExpr1) {
    std:: map<std::string, std::shared_ptr<DataBaseType>> row = {{"val", std::make_shared<Int>(Int("-12"))},
                                                 {"login", std::make_shared<String>(String("maxim"))},
                                                 {"password", std::make_shared<Bytes>(Bytes("0xaacf1242f"))},
                                                 {"is_cool", std::make_shared<Bool>(Bool("true"))},
                                                 {"salary", std::make_shared<Int>(Int("5"))},
                                                 {"name", std::make_shared<String>(String("artem"))}};
    std::string val = " 1  + val - ( -val *  10 )  = -131";
    ASSERT_EQ(*static_cast<bool*>(parseExpr(val, row)->type), true);
    val = "log + (login + log)";
    ASSERT_EQ(*static_cast<std::string*>(parseExpr(val, row)->type), "logmaximlog");
    val = "val < 0 && login = maxim && (false || password < 0xaacf1243f)";
    ASSERT_EQ(*static_cast<bool*>(parseExpr(val, row)->type), true);
    val = "true || false && false";
    ASSERT_EQ(*static_cast<bool*>(parseExpr(val, row)->type), true);
    val = "(name + \"bye\") + login";
    ASSERT_EQ(*static_cast<std::string*>(parseExpr(val, row)->type), "artembyemaxim");
    val = "-val * (salary - 2) % 3";
    ASSERT_EQ(*static_cast<int*>(parseExpr(val, row)->type), 0);
}
