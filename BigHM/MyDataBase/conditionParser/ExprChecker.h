//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_EXPRCHECKER_H
#define CPP_SEMINARS_EXPRCHECKER_H
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <functional>
#include <stdexcept>
#include <regex>
#include <iomanip>
#include <cmath>
#include "../domain/DataBaseType.h"
#include "../domain/ColTable.h"

std::shared_ptr<DataBaseType> checkExpr(const std::string& expr, std::map<std::string, std::shared_ptr<DataBaseType>>& row);
std::shared_ptr<DataBaseType> doBoolOp(const std::string& comp, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal);
std::shared_ptr<DataBaseType> doCompare(std::string& comp, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal);
std::shared_ptr<DataBaseType> getVal(std::vector<std::string>& v, int idx, std::map<std::string, std::shared_ptr<DataBaseType>>& row);
std::shared_ptr<DataBaseType> doSimpleOp(std::string op, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal);
std::shared_ptr<DataBaseType> doOp(std::string op, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal);
std::shared_ptr<DataBaseType> getByString(std::string val, std::map<std::string, std::shared_ptr<DataBaseType>>& row);
std::string getStringByType(std::shared_ptr<DataBaseType> val);
std::shared_ptr<DataBaseType> parseExpr(const std::string& s, std::map<std::string, std::shared_ptr<DataBaseType>>& row);
std::shared_ptr<DataBaseType> doSDNF(const std::string& s, std::map<std::string, std::shared_ptr<DataBaseType>>& row);
bool isNumber(std::string& s);
bool isCompareOp(std::string& s);
bool isBoolOp(std::string& s);
class ExprChecker {
public:
    static std::shared_ptr<DataBaseType> getValFromExpr(const std::string& expr, std::vector<std::shared_ptr<DataBaseType>> row, std::vector<Col> columns);
    static bool check(const std::string& expr, std::vector<std::shared_ptr<DataBaseType>> row, std::vector<Col> columns);
};

#endif //CPP_SEMINARS_EXPRCHECKER_H
