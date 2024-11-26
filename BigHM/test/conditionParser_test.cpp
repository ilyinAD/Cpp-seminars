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
                                                 {"is_cool", std::make_shared<Bool>(Bool("true"))}};
    std::string val = "1 + val - (-val * 10) = -131";
    ASSERT_EQ(*static_cast<bool*>(parseExpr(val, row)->type), true);
}

TEST(conditionParser, parseExpr2) {
    std:: map<std::string, std::shared_ptr<DataBaseType>> row = {{"val", std::make_shared<Int>(Int("-12"))},
                                                                 {"login", std::make_shared<String>(String("maxim"))},
                                                                 {"password", std::make_shared<Bytes>(Bytes("0xaacf1242f"))},
                                                                 {"is_cool", std::make_shared<Bool>(Bool("true"))}};
    std::string val = "log + (login + log)";
    ASSERT_EQ(*static_cast<std::string*>(parseExpr(val, row)->type), "logmaximlog");
}

TEST(conditionParser, parseExpr3) {
    std:: map<std::string, std::shared_ptr<DataBaseType>> row = {{"val", std::make_shared<Int>(Int("-12"))},
                                                                 {"login", std::make_shared<String>(String("maxim"))},
                                                                 {"password", std::make_shared<Bytes>(Bytes("0xaacf1242f"))},
                                                                 {"is_cool", std::make_shared<Bool>(Bool("true"))}};
    std::string val = "val < 0 && login = maxim && (false || password < 0xaacf1243f)";
    ASSERT_EQ(*static_cast<bool*>(parseExpr(val, row)->type), true);
}

TEST(conditionParser, parseExpr4) {
    std:: map<std::string, std::shared_ptr<DataBaseType>> row = {{"val", std::make_shared<Int>(Int("-12"))},
                                                                 {"login", std::make_shared<String>(String("maxim"))},
                                                                 {"password", std::make_shared<Bytes>(Bytes("0xaacf1242f"))},
                                                                 {"is_cool", std::make_shared<Bool>(Bool("true"))}};
    std::string val = "true || false && false";
    ASSERT_EQ(*static_cast<bool*>(parseExpr(val, row)->type), true);
}