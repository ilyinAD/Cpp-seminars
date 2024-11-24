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
    } else if (col.name == TypeName::Bytes) {
        return make_shared<Bytes>(Bytes(value, col.size));
    } else {
        if (value.size() > col.size) {
            throw invalid_argument("Invalid size of string");
        }
        return make_shared<String>(String(value, col.size));
    }
}

string toLower(const string& s) {
    string ans;
    for (auto i : s) {
        ans += tolower(i);
    }
    return ans;
}

string deleteDoubleSpaces(const string& s) {
    string ans;
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

string getCommand(const string& s, int idx) {
    string ans;
    for (int i = idx; i < s.size(); ++i) {
        if (s[idx] == ' ') {
            break;
        }
        ans += s[i];
    }
    return ans;
}

string deleteSpaces(const string& s) {
    string ans;
    for (auto i : s) {
        if (i != ' ' && i != '\n') {
            ans += i;
        }
    }

    return ans;
}

string deleteCornerSpaces(const string& s) {
    string ans = "";
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