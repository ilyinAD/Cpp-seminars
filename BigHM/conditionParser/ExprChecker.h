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
using namespace std;
shared_ptr<DataBaseType> checkExpr(const string& expr, map<string, shared_ptr<DataBaseType>>& row);
shared_ptr<DataBaseType> doBoolOp(string& comp, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal);
shared_ptr<DataBaseType> doCompare(string& comp, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal);
shared_ptr<DataBaseType> getVal(vector<string>& v, int idx, map<string, shared_ptr<DataBaseType>>& row);
shared_ptr<DataBaseType> doSimpleOp(string op, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal);
shared_ptr<DataBaseType> doOp(string op, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal);
shared_ptr<DataBaseType> getByString(string val, map<string, shared_ptr<DataBaseType>>& row);
string getStringByType(shared_ptr<DataBaseType> val);
shared_ptr<DataBaseType> parseExpr(const string& s, map<string, shared_ptr<DataBaseType>>& row);

bool isNumber(string& s);
bool isCompareOp(string& s);
bool isBoolOp(string& s);
class ExprChecker {
public:
    static shared_ptr<DataBaseType> getValFromExpr(const string& expr, vector<shared_ptr<DataBaseType>> row, vector<Col> columns);
    static bool check(const string& expr, vector<shared_ptr<DataBaseType>> row, vector<Col> columns);
};

#endif //CPP_SEMINARS_EXPRCHECKER_H
