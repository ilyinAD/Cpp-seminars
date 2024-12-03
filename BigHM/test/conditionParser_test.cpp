//
// Created by artem on 25.11.2024.
//
#include "gtest/gtest.h"
#include "conditionParser/ExprChecker.h"
#include <map>

TEST(conditionParser, parseExpr) {
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
    val = "name + \"||\"";
    ASSERT_EQ(*static_cast<std::string*>(parseExpr(val, row)->type), "artem||");
}

TEST(conditionParser, getBystring) {
    std::map<std::string, std::shared_ptr<DataBaseType>> m = {{"name", std::make_shared<String>("max")},
                                                              {"val", std::make_shared<Int>(Int("10"))},
                                                              {"is_ok", std::make_shared<Bool>(Bool("true"))}};
    std::shared_ptr<DataBaseType> val = getByString("-val", m);
    ASSERT_EQ("-10", getStringByType(val));

    val = getByString("!is_ok", m);
    ASSERT_EQ("false", getStringByType(val));

    val = getByString("|name|", m);
    ASSERT_EQ("3", getStringByType(val));

    val = getByString("!is_ok", m);
    ASSERT_EQ("false", getStringByType(val));
}

TEST(conditionParser, getVal) {
    std::map<std::string, std::shared_ptr<DataBaseType>> m = {{"name", std::make_shared<String>("max")},
                                                              {"val", std::make_shared<Int>(Int("10"))},
                                                              {"is_ok", std::make_shared<Bool>(Bool("true"))}};
    std::vector<std::string> v = {"val", "+", "11", "*", "val", "%", "3"};
    ASSERT_EQ("12", getStringByType(getVal(v, 0, m)));
}

TEST(conditionParser, doSDNF) {
    std::map<std::string, std::shared_ptr<DataBaseType>> m = {{"name", std::make_shared<String>("max")},
                                                              {"val", std::make_shared<Int>(Int("10"))},
                                                              {"is_ok", std::make_shared<Bool>(Bool("true"))}};
    ASSERT_EQ("false", getStringByType(doSDNF("val + 10 <= 20 && is_ok && is_ok ^^ is_ok", m)));
    ASSERT_EQ("true", getStringByType(doSDNF("val + 10 <= 20 && is_ok", m)));
}

TEST(conditionParser, checkExpr) {
    std::map<std::string, std::shared_ptr<DataBaseType>> m = {{"name", std::make_shared<String>("max")},
                                                              {"val", std::make_shared<Int>(Int("10"))},
                                                              {"is_ok", std::make_shared<Bool>(Bool("true"))}};
    ASSERT_EQ("true", getStringByType(checkExpr("val + 10 <= 20 && is_ok || is_ok ^^ is_ok", m)));
    ASSERT_EQ("false", getStringByType(checkExpr("!is_ok || true && !is_ok", m)));
}
