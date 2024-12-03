//
// Created by artem on 17.11.2024.
//

#include "utils.h"

std::vector<std::string> splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        if (token.size() == 0) {
            continue;
        }
        tokens.push_back(token);
    }

    return tokens;
}

std::shared_ptr<DataBaseType> getTypeByCol(Type col, std::string value) {
    if (col.name == TypeName::Int) {
        return std::make_shared<Int>(Int(value));
    } else if (col.name == TypeName::Bool) {
        return std::make_shared<Bool>(Bool(value));
    } else if (col.name == TypeName::Bytes) {
        return std::make_shared<Bytes>(Bytes(value, col.size));
    } else {
//        if (value.size() > col.size) {
//            throw std::invalid_argument("Invalid size of string");
//        }
        return std::make_shared<String>(String(value, col.size));
    }
}

std::string toLower(const std::string& s) {
    std::string ans = s;
    std::transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
    return ans;
}

std::string deleteDoubleSpaces(const std::string& s) {
    std::string ans;
    char prev = '_';
    for (auto i : s) {
        if (i == ' ' && prev == ' ') {
            continue;
        }
        prev = i;
        ans += i;
    }

    return ans;
}

std::string getCommand(const std::string& s, int idx) {
    std::string ans;
    for (int i = idx; i < s.size(); ++i) {
        if (s[i] == ' ') {
            break;
        }
        ans += s[i];
    }
    return ans;
}

std::string deleteSpaces(const std::string& s) {
    std::string ans;
    for (auto i : s) {
        if (i != ' ' && i != '\n') {
            ans += i;
        }
    }

    return ans;
}

std::string deleteCornerSpaces(const std::string& s) {
    std::string ans = "";
    int idx = 0;
    while (idx < s.size() && (s[idx] == ' ' || s[idx] == '\n')) {
        ++idx;
    }
    int idx1 = s.size() - 1;
    while (idx1 > idx && (s[idx1] == ' ' || s[idx1] == '\n')) {
        --idx1;
    }
    for (int i = idx; i <= idx1; ++i) {
        ans += s[i];
    }
    return ans;
}

std::string getStringByType(std::shared_ptr<DataBaseType> val) {
    if (dynamic_pointer_cast<String>(val)) {
        return *static_cast<std::string*>(val->type);
    } else if (dynamic_pointer_cast<Bytes>(val)) {
        return "0x" + *static_cast<std::string*>(val->type);
    } else if (dynamic_pointer_cast<Bool>(val)) {
        bool b = (*static_cast<bool*>(val->type));
        if ((*static_cast<bool*>(val->type))) {
            return "true";
        } else {
            return "false";
        }
    } else {
        return std::to_string(*static_cast<int*>(val->type));
    }
}