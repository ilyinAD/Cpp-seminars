//
// Created by artem on 17.11.2024.
//

#include "ExprChecker.h"
#include "../utils/utils.h"

bool isBoolOp(string& s) {
    if (s == "&&" || s == "||" || s == "^^") {
        return true;
    }

    return false;
}

bool isCompareOp(string& s) {
    if (s == "=" || s == "<" || s == "!=" || s == ">" || s == ">=" || s == "<=") {
        return true;
    }

    return false;
}

bool isNumber(string& s) {
    for (auto i : s) {
        if (!isdigit(i)) {
            return false;
        }
    }

    return true;
}

shared_ptr<DataBaseType> getByString(string val, map<string, shared_ptr<DataBaseType>>& row) {
    if (isNumber(val)) {
        return make_shared<Int>(Int(val));
    } else if (val == "true" || val == "false") {
        return make_shared<Bool>(Bool(val));
    } else if (val.size() > 2 && val[0] == '0' && val[1] == 'x') {
        return make_shared<Bytes>(Bytes(val));
    } else if (val[0] == '"' && val[val.size() - 1] == '"') {
        return make_shared<String>(String(val.substr(1, val.size() - 1)));
    } else {
        if (val[0] != '|') {
            if (row.find(val) != row.end()) {
                return row[val];
            }
        } else {
            if (val.size() < 2) {
                return make_shared<String>(String(val));
            }

            string val1 = val.substr(1, val.size() - 2);
            if (row.find(val1) != row.end()) {
                if (dynamic_pointer_cast<String>(row[val1])) {
                    auto s = static_cast<string*>((*row[val1]).type);
                    return make_shared<Int>(Int(static_cast<int>(s->size())));
                }
                if (dynamic_pointer_cast<Bytes>(row[val1])) {
                    auto s = static_cast<string*>((*row[val1]).type);
                    return make_shared<Int>(Int(static_cast<int>(s->size())));
                }
            }

            return make_shared<Int>(Int(static_cast<int>(val1.size())));
        }

        return make_shared<String>(String(val));
    }
}

shared_ptr<DataBaseType> doOp(string op, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (op == "*")
        d = ((*leftVal) * (*rightVal));
    else if (op == "/")
        d = ((*leftVal) / (*rightVal));
    else if (op == "%")
        d = ((*leftVal) % (*rightVal));
    else
        throw invalid_argument("invalid operation sign");
    shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

shared_ptr<DataBaseType> doSimpleOp(string op, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (op == "+")
        d = ((*leftVal) + (*rightVal));
    else
        d = ((*leftVal) - (*rightVal));
    shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

shared_ptr<DataBaseType> getVal(vector<string>& v, int idx, map<string, shared_ptr<DataBaseType>>& row) {
    if (idx >= v.size()) {
        return make_shared<Int>(Int(0));
    }
    if (idx == v.size() - 1) {
        return getByString(v[idx], row);
    }

    if (v[idx + 1] == "+" || v[idx + 1] == "-") {
        shared_ptr<DataBaseType> leftVal = getByString(v[idx], row);
        shared_ptr<DataBaseType> rightVal = getVal(v, idx + 2, row);
        return doSimpleOp(v[idx + 1], leftVal, rightVal);
    } else if (v[idx + 1] == "*" || v[idx + 1] == "/" || v[idx + 1] == "%") {
        shared_ptr<DataBaseType> cur = getByString(v[idx], row);
        while (idx + 2 < v.size()) {
            if (v[idx + 1] == "+" || v[idx + 1] == "-") {
                shared_ptr<DataBaseType> rightVal = getVal(v, idx + 2, row);
                return doSimpleOp(v[idx + 1], cur, rightVal);
            } else {
                cur = doOp(v[idx + 1], cur, getByString(v[idx + 2], row));
                idx += 2;
            }
        }
        return cur;
    } else {
        throw invalid_argument("invalid operation");
    }
}

shared_ptr<DataBaseType> doCompare(string& comp, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (comp == "<")
        d = ((*leftVal) < (*rightVal));
    else if (comp == ">")
        d = ((*leftVal) > (*rightVal));
    else if (comp == "=")
        d = ((*leftVal) == (*rightVal));
    else if (comp == "!=")
        d = ((*leftVal) != (*rightVal));
    else if (comp == "<=")
        d = ((*leftVal) <= (*rightVal));
    else if (comp == ">=")
        d = ((*leftVal) >= (*rightVal));
    else
        throw invalid_argument("invalid compare sign");
    shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

shared_ptr<DataBaseType> doBoolOp(string& comp, const shared_ptr<DataBaseType>& leftVal, const shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (comp == "&&")
        d = ((*leftVal) && (*rightVal));
    else if (comp == "||")
        d = ((*leftVal) || (*rightVal));
    else if (comp == "^^")
        d = ((*leftVal) ^ (*rightVal));
    shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

shared_ptr<DataBaseType> checkExpr(const string& expr, map<string, shared_ptr<DataBaseType>>& row) {
    vector<string> v = splitString(expr, ' ');
    v.push_back("&&");
    v.push_back("1");
    vector<string> query;
    string prev = "";
    shared_ptr<DataBaseType> res = nullptr;
    for (int i = 0; i < v.size(); ++i) {
        if (!isBoolOp(v[i])) {
            query.push_back(v[i]);
        } else {
            vector<string> query1;
            vector<string> query2;
            int flag = 0;
            string comp = "";
            for (int j = 0; j < query.size(); ++j) {
                if (isCompareOp(query[j])) {
                    flag = 1;
                    comp = query[j];
                    continue;
                }
                if (!flag) {
                    query1.push_back(query[j]);
                } else {
                    query2.push_back(query[j]);
                }
            }

            shared_ptr<DataBaseType> q1 = getVal(query1, 0, row);
            shared_ptr<DataBaseType> q2 = getVal(query2, 0, row);

//            if (i == v.size() - 1) {
//                if (comp == "") {
//                    return q1;
//                }
//                shared_ptr<DataBaseType> res1 = doCompare(comp, q1, q2);
//                return res1;
//            }

//            if (comp == "") {
//                comp = "=";
//                q2 = make_shared<Bool>(Bool("True"));
//            }
//            shared_ptr<DataBaseType> res1 = doCompare(comp, q1, q2);
            shared_ptr<DataBaseType> res1;
            if (comp != "") {
                res1 = doCompare(comp, q1, q2);
            } else {
                res1 = q1;
            }
            if (res != nullptr) {
                res = doBoolOp(prev, res, res1);
            } else {
                res = res1;
            }

            prev = v[i];
            query = {};
        }
    }
//    if (dynamic_pointer_cast<Bool>(res)) {
//        return *(static_cast<bool*>(res.get()->type));
//    }
    return res;
    throw invalid_argument("шото не то хз");
}

shared_ptr<DataBaseType> ExprChecker::getValFromExpr(const std::string &expr, vector<shared_ptr<DataBaseType>> row,
                                                     vector<Col> columns) {
    map<string, shared_ptr<DataBaseType>> mapRow;
    for (int i = 0; i < columns.size(); ++i) {
        mapRow[columns[i].name] = row[i];
    }
    try {
        shared_ptr<DataBaseType> res = checkExpr(expr, mapRow);
        return res;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}

bool ExprChecker::check(const std::string &expr, vector<shared_ptr<DataBaseType>> row, vector<Col> columns) {
    map<string, shared_ptr<DataBaseType>> mapRow;
    for (int i = 0; i < columns.size(); ++i) {
        mapRow[columns[i].name] = row[i];
    }
    try {
        shared_ptr<DataBaseType> res = checkExpr(expr, mapRow);
        if (dynamic_pointer_cast<Bool>(res)) {
            return *(static_cast<bool*>(res.get()->type));
        }
       if (dynamic_pointer_cast<Int>(res)) {
           int q = *(static_cast<int*>(res.get()->type));
           return static_cast<bool>(q);
       }
       throw invalid_argument("result is not bool and not int");
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}
