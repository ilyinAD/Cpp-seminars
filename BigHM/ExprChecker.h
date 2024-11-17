//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_EXPRCHECKER_H
#define CPP_SEMINARS_EXPRCHECKER_H

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
#include "DataBase.h"
using namespace std;

enum class TokenType {
    COLUMN,
    NUMBER,
    OPERATOR
};

struct Token {
    TokenType type;
    string value;
};

struct Node {
    string value;
    Node* left;
    Node* right;

    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

vector<Token> tokenize(const string& expression) {
    vector<Token> tokens;

    regex column_regex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
    regex number_regex(R"(\d+(\.\d+)?)");
    regex operator_regex(R"([+\-*/%<>=!]+)");

    auto begin = expression.cbegin();
    auto end = expression.cend();

    while (begin != end) {
        if (isspace(*begin)) {
            ++begin;
            continue;
        }

        smatch match;

        if (regex_search(begin, end, match, column_regex)) {
            tokens.push_back({TokenType::COLUMN, match.str()});
            begin += match.length();
        }
        else if (regex_search(begin, end, match, number_regex)) {
            tokens.push_back({TokenType::NUMBER, match.str()});
            begin += match.length();
        }
        else if (regex_search(begin, end, match, operator_regex)) {
            tokens.push_back({TokenType::OPERATOR, match.str()});
            begin += match.length();
        }
        else {
            throw invalid_argument("Invalid token found in expression");
        }
    }

    return tokens;
}

Node* buildAST(const vector<Token>& tokens) {
    vector<Node*> stack;

    for (const auto& token : tokens) {
        if (token.type == TokenType::COLUMN || token.type == TokenType::NUMBER) {
            stack.push_back(new Node(token.value));
        } else if (token.type == TokenType::OPERATOR) {
            if (stack.size() < 2) {
                throw invalid_argument("Invalid expression");
            }

            Node* right = stack.back(); stack.pop_back();
            Node* left = stack.back(); stack.pop_back();
            Node* operatorNode = new Node(token.value);
            operatorNode->left = left;
            operatorNode->right = right;

            stack.push_back(operatorNode);
        } else {
            throw invalid_argument("Unexpected token type");
        }
    }

    if (stack.size() != 1) {
        throw invalid_argument("Invalid expression");
    }

    return stack.back();
}

shared_ptr<DataBaseType> evaluateAST(Node* node, const map<string, shared_ptr<DataBaseType>>& row) {
    if (!node) {
        return 0;
    }

    if (node->left == nullptr && node->right == nullptr) {
        if (isdigit(node->value[0])) {
            Int* q = new Int(stoi(node->value));
            shared_ptr<DataBaseType> sharedPtr(q);
            return sharedPtr;
        } else {
            auto it = row.find(node->value);
            if (it != row.end()) {
                return it->second;
            } else {
                throw invalid_argument("Column not found: " + node->value);
            }
        }
    }

    shared_ptr<DataBaseType> leftVal = evaluateAST(node->left, row);
    shared_ptr<DataBaseType> rightVal = evaluateAST(node->right, row);

    if (node->value == "+") {
        DataBaseType* d = ((*leftVal.get()) + (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == "-") {
        DataBaseType* d = ((*leftVal.get()) - (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == "*") {
        DataBaseType* d = ((*leftVal.get()) * (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == "/") {
        if (rightVal == 0) {
            throw runtime_error("Division by zero");
        }
        DataBaseType* d = ((*leftVal.get()) / (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == "%") {
        DataBaseType* d = ((*leftVal.get()) % (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == "==") {
        DataBaseType* d = ((*leftVal.get()) == (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == "!=") {
        DataBaseType* d = ((*leftVal.get()) != (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == "<") {
        DataBaseType* d = ((*leftVal.get()) < (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else if (node->value == ">") {
        DataBaseType* d = ((*leftVal.get()) > (*rightVal.get()));
        shared_ptr<DataBaseType> sharedPtr(d);
        return sharedPtr;
    } else {
        throw invalid_argument("Unknown operator: " + node->value);
    }
}

void deleteAST(Node* root) {
    if (root) {
        deleteAST(root->left);
        deleteAST(root->right);
        delete root;
    }
}


class ExprChecker {
    static bool check(const string& expr, map<string, shared_ptr<DataBaseType>> row) {
        try {
            std::vector<Token> tokens = tokenize(expr);

            Node* root = buildAST(tokens);

            shared_ptr<DataBaseType> result = evaluateAST(root, row);

            deleteAST(root);
            if (dynamic_pointer_cast<Bool>(result)) {
                return *(static_cast<bool*>(result.get()->type));
            }

            throw invalid_argument("wrong expr");
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
};

#endif //CPP_SEMINARS_EXPRCHECKER_H
