//
// Created by artem on 16.11.2024.
//
#include "DataBase.h"

pair<vector<Element>, string> DataBase::parseCreate(const string& expr) {
    vector<Element> elements;
    string table_name, fields_section;
    size_t start_pos = expr.find('(');
    size_t end_pos = expr.find(')');
    table_name = expr.substr(0, start_pos - 1);
    fields_section = expr.substr(start_pos + 1, end_pos - start_pos - 1);

    istringstream stream(fields_section);
    string col;

    while (getline(stream, col, ',')) {
        Element element;
        Attributes attributes;
        size_t brace_start = col.find('{');
        size_t brace_end = col.find('}');
        if (brace_start != string::npos && brace_end != string::npos) {
            string attr_section = col.substr(brace_start + 1, brace_end - brace_start - 1);
            if (attr_section.find("key") != string::npos) attributes.is_key = true;
            if (attr_section.find("autoincrement") != string::npos) attributes.is_autoincrement = true;
            if (attr_section.find("unique") != string::npos) attributes.is_unique = true;
            col = col.substr(brace_end + 2);
        }

        size_t colon_pos = col.find(':');
        string name = col.substr(0, colon_pos);
        string value = col.substr(colon_pos + 2);

        size_t equals_pos = value.find('=');
        if (equals_pos != string::npos) {
            attributes.default_value = value.substr(equals_pos + 1);
            value = value.substr(0, equals_pos);
        }

        element.name = name;
        element.value = value;
        element.attributes = attributes;
        elements.push_back(element);
    }
    for (auto& i : elements) {
        i.name = deleteCornerSpaces(i.name);
        i.value = deleteCornerSpaces(i.value);
        i.attributes.default_value = deleteCornerSpaces(i.attributes.default_value);
    }
    return {elements, table_name};
}

string change(const string& s) {
    string ans = "";
    for (int i = 0; i < s.size(); ++i) {
        size_t open_bracket = s.find('{', i);
        size_t close_bracket = s.find('}', i);
        if (s[i] == ',' && open_bracket > close_bracket) {
            ans += ';';
        } else {
            ans += s[i];
        }
    }
    return ans;
}

void DataBase::create(string s) {
    s = toLower(s);
    s = deleteDoubleSpaces(s);
    s = s.substr(13);
    s = change(s);
    pair<vector<Element>, string> p = parseCreate(s);
    tables[p.second] = Table(p.second, p.first);
}

pair<string, vector<string>> parseInsert(const string& expression) {
    string tableName;
    vector<string> arguments;

    size_t toPos = expression.find(" to ");
    if (toPos != string::npos) {
        size_t tableStart = toPos + 3;
        tableName = expression.substr(tableStart);
    }

    size_t endBracket_pos = expression.find(')');
    string args = expression.substr(1, endBracket_pos - 1);
    istringstream stream(args);
    string row;
    while (getline(stream, row, ',')) {
        arguments.push_back(row);
    }

    return {deleteCornerSpaces(tableName), arguments};
}

void DataBase::insert(string s) {
    s = toLower(s);
    s = deleteDoubleSpaces(s);
    s = s.substr(7);
    s.erase(std::remove(s.begin(), s.end(), '"'), s.end());
    pair<string, vector<string>> p = parseInsert(s);
    vector<string> v = p.second;
    string table_name = p.first;
    map<string, string> m;
    if (s.find('=') < s.size()) {
        for (int i = 0; i < v.size(); ++i) {
            size_t equals_pos = v[i].find('=');
            string colName = deleteCornerSpaces(v[i].substr(0, equals_pos));
            string val = deleteCornerSpaces(v[i].substr(equals_pos + 1));
            m[colName] = val;
        }
    } else {
        for (int i = 0; i < v.size(); ++i) {
            string s = deleteCornerSpaces(v[i]);
            if (s.size() == 0) {
                continue;
            }
            string colName = tables[table_name].columns[i].name;
            m[colName] = s;
        }
    }
    tables[table_name].insert(m);
}

void DataBase::deleteRows(std::string s) {
    s = s.substr(7);
    size_t space_pos = s.find(' ');
    string table_name = deleteCornerSpaces(s.substr(0, space_pos));
    s = s.substr(space_pos + 1);
    s = s.substr(6);
    tables[table_name].deleteRows(deleteCornerSpaces(s));
}

Table DataBase::handleJoin(std::string s) {
    string name1 = s.substr(0, s.find(' '));
    name1 = deleteCornerSpaces(name1);
    Table table = tables[name1];
    while (s.find("join") < s.size()) {
        size_t firstJoin = s.find("join");
        size_t onCond = s.find("on");
        string name2 = s.substr(firstJoin + 4, onCond - firstJoin - 4);
        name2 = deleteCornerSpaces(name2);
        s = s.substr(onCond + 3);
        size_t secondJoin = s.find("join");
        string onExpr = s.substr(0, secondJoin  - 1);
        Table table2 = tables[name2];
        table = table.join(table, table2, onExpr, name1 + name2);
        name1 = table.name;
    }
    return table;
}

Table DataBase::select(string s) {
    s = s.substr(7);
    size_t from = s.find("from");
    vector<string> colNames;
    string curName = "";
    string names = s.substr(0, from);
    for (int i = 0; i < names.size(); ++i) {
        if (names[i] != ',') {
            curName += names[i];
        } else {
            colNames.push_back(deleteCornerSpaces(curName));
            curName = "";
        }
    }
    if (curName != "")
        colNames.push_back(deleteCornerSpaces(curName));
    size_t where = s.find("where");
    string whereCond = deleteCornerSpaces(s.substr(where + 6));
    s = s.substr(from + 5, where - 1 - from - 5);
    Table table = handleJoin(s);
    return table.select(colNames, whereCond, "");
}

Table DataBase::update(string s) {
    s = s.substr(7);
    size_t set = s.find("set");
    string s1 = s.substr(0, set - 1);
    Table table = handleJoin(s1);
    size_t where = s.find("where");
    string whereCond = s.substr(where + 6);
    s = s.substr(set + 4, where - 1 - set - 4);
    istringstream stream(s);
    string row;
    vector<string> v;
    while (getline(stream, row, ',')) {
        v.push_back(row);
    }

    map<string, string> m;
    for (int i = 0; i < v.size(); ++i) {
        size_t equals_pos = v[i].find('=');
        string colName = deleteCornerSpaces(v[i].substr(0, equals_pos));
        string val = deleteCornerSpaces(v[i].substr(equals_pos + 1));
        m[colName] = val;
    }

    table.update(m, whereCond);

    return table;
}