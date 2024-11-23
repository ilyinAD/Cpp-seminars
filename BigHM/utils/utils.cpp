//
// Created by artem on 17.11.2024.
//

#include "utils.h"

vector<string> splitString(const string &str, char delimiter) {
    vector<std::string> tokens;
    string token;
    istringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

shared_ptr<DataBaseType> getTypeByCol(Type col, string value) {
    if (col.name == TypeName::Int) {
        return make_shared<Int>(Int(value));
    } else if (col.name == TypeName::Bool) {
        return make_shared<Bool>(Bool(value));
    } else {
        if (value.size() > col.size) {
            throw invalid_argument("Invalid size of string");
        }
        return make_shared<String>(String(value, col.size));
    }
}