//
// Created by artem on 16.11.2024.
//
#include "DataBase.h"

std::pair<std::vector<Element>, std::string> DataBase::parseCreate(const std::string& expr) {
    std::vector<Element> elements;
    std::string table_name, fields_section;
    size_t start_pos = expr.find('(');
    size_t end_pos = expr.find(')');
    table_name = expr.substr(0, start_pos - 1);
    fields_section = expr.substr(start_pos + 1, end_pos - start_pos - 1);

    std::istringstream stream(fields_section);
    std::string col;

    while (getline(stream, col, ',')) {
        Element element;
        Attributes attributes;
        size_t brace_start = col.find('{');
        size_t brace_end = col.find('}');
        if (brace_start != std::string::npos && brace_end != std::string::npos) {
            std::string attr_section = col.substr(brace_start + 1, brace_end - brace_start - 1);
            if (attr_section.find("key") != std::string::npos) attributes.is_key = true;
            if (attr_section.find("autoincrement") != std::string::npos) attributes.is_autoincrement = true;
            if (attr_section.find("unique") != std::string::npos) attributes.is_unique = true;
            col = col.substr(brace_end + 2);
        }

        size_t colon_pos = col.find(':');
        std::string name = col.substr(0, colon_pos);
        std::string value = col.substr(colon_pos + 2);

        size_t equals_pos = value.find('=');
        if (equals_pos != std::string::npos) {
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

std::string change(const std::string& s) {
    std::string ans = "";
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

void DataBase::create(std::string s) {
    s = deleteDoubleSpaces(s);
    s = s.substr(13);
    s = change(s);
    std::pair<std::vector<Element>, std::string> p = parseCreate(s);
    tables[p.second] = Table(p.second, p.first);
}

std::pair<std::string, std::vector<std::string>> parseInsert(const std::string& expression) {
    std::string tableName;
    std::vector<std::string> arguments;

    size_t toPos = expression.find(" to ");
    if (toPos != std::string::npos) {
        size_t tableStart = toPos + 3;
        tableName = expression.substr(tableStart);
    }

    size_t endBracket_pos = expression.find(')');
    std::string args = expression.substr(1, endBracket_pos - 1);
    std::istringstream stream(args);
    std::string row;
    while (getline(stream, row, ',')) {
        arguments.push_back(row);
    }

    return {deleteCornerSpaces(tableName), arguments};
}

std::pair<std::string, std::vector<std::string>> changeQuote(std::string& s) {
    std::string ans;
    std::vector<std::string> v;
    int i = 0;
    while (i < s.size()) {
        if (s[i] != '"') {
            ans += s[i];
            ++i;
            continue;
        }
        ++i;
        std::string cur = "";
        while (i < s.size() && s[i] != '"') {
            cur += s[i];
            ++i;
        }
        ++i;
        v.push_back(cur);
        ans += '"';
    }

    return std::make_pair(ans, v);
}

void DataBase::insert(std::string s) {
    s = deleteDoubleSpaces(s);
    s = s.substr(7);
    std::pair<std::string, std::vector<std::string>> chngPair = changeQuote(s);
    s = chngPair.first;
    std::pair<std::string, std::vector<std::string>> p = parseInsert(s);
    std::vector<std::string> v = p.second;
    std::string table_name = p.first;
    std::map<std::string, std::string> m;
    int idx = 0;
    if (s.find('=') < s.size()) {
        for (int i = 0; i < v.size(); ++i) {
            size_t equals_pos = v[i].find('=');
            std::string colName = deleteCornerSpaces(v[i].substr(0, equals_pos));
            std::string val = deleteCornerSpaces(v[i].substr(equals_pos + 1));
            if (val == "\"") {
                val = chngPair.second[idx];
                ++idx;
            }
            m[colName] = val;
        }
    } else {
        for (int i = 0; i < v.size(); ++i) {
            std::string s = deleteCornerSpaces(v[i]);
            if (s.size() == 0) {
                continue;
            }
            std::string colName = tables[table_name].columns[i].name;
            if (s == "\"") {
                s = chngPair.second[idx];
                ++idx;
            }
            m[colName] = s;
        }
    }
    tables[table_name].insert(m);
}

void DataBase::deleteRows(std::string s) {
    s = s.substr(7);
    size_t space_pos = s.find(' ');
    std::string table_name = deleteCornerSpaces(s.substr(0, space_pos));
    s = s.substr(space_pos + 1);
    s = s.substr(6);
    tables[table_name].deleteRows(deleteCornerSpaces(s));
}

Table DataBase::handleJoin(std::string s) {
    std::string name1 = s.substr(0, s.find(' '));
    name1 = deleteCornerSpaces(name1);
    Table table = tables[name1];
    while (s.find("join") < s.size()) {
        size_t firstJoin = s.find(" join ");
        size_t onCond = s.find(" on ");
        std::string name2 = s.substr(firstJoin + 6, onCond - firstJoin - 6);
        name2 = deleteCornerSpaces(name2);
        s = s.substr(onCond + 4);
        size_t secondJoin = s.find("join");
        std::string onExpr = s.substr(0, secondJoin  - 1);
        Table table2 = tables[name2];
        table = table.join(table, table2, onExpr, name1 + name2);
        name1 = table.name;
    }
    return table;
}

Table DataBase::select(std::string s) {
    s = s.substr(7);
    size_t from = s.find("from");
    std::vector<std::string> colNames;
    std::string curName = "";
    std::string names = s.substr(0, from);
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
    std::string whereCond = deleteCornerSpaces(s.substr(where + 6));
    s = s.substr(from + 5, where - 1 - from - 5);
    Table table = handleJoin(s);
    return table.select(colNames, whereCond, "");
}

void replaceAll(std::string& str, const std::string& needChange, const std::string& onChange, size_t pos) {
    if (needChange.empty()) {
        return;
    }
    if ((pos = str.find(needChange, pos)) != std::string::npos) {
        str.replace(pos, needChange.length(), onChange);
    }
}

Table DataBase::update(std::string s) {
    s = s.substr(7);
    size_t set = s.find("set");
    std::string s1 = s.substr(0, set - 1);
    std::pair<std::string, std::vector<std::string>> chngPair = changeQuote(s);
    s = chngPair.first;
    Table table = handleJoin(s1);
    size_t where = s.find("where");
    std::string whereCond = s.substr(where + 6);
    s = s.substr(set + 4, where - 1 - set - 4);
    std::istringstream stream(s);
    std::string row;
    std::vector<std::string> v;
    while (getline(stream, row, ',')) {
        v.push_back(row);
    }
    int idx = 0;
    std::map<std::string, std::string> m;
    for (int i = 0; i < v.size(); ++i) {
        size_t equals_pos = v[i].find('=');
        std::string colName = deleteCornerSpaces(v[i].substr(0, equals_pos));
        std::string val = deleteCornerSpaces(v[i].substr(equals_pos + 1));

        size_t pos = 0;
        while ((pos = val.find("\"", pos)) < val.size()) {
            replaceAll(val, "\"", "\"" + chngPair.second[idx] + "\"", pos);
            pos += chngPair.second[idx].size() + 2;
            ++idx;

        }

        m[colName] = val;
    }

    table.update(m, whereCond);

    return table;
}

void DataBase::execute(std::string s) {
    my_mutex.lock();
    std::istringstream stream(s);
    std::string query;
    while (getline(stream, query, ';')) {
        std::string typeQuery = query.substr(0, query.find(' '));
        try {
            if (typeQuery == "insert") {
                insert(query);
            } else if (typeQuery == "update") {
                update(query);
            } else if (typeQuery == "create") {
                create(query);
            } else if (typeQuery == "select") {
                select(query);
            } else if (typeQuery == "delete") {
                deleteRows(query);
            } else {
                throw std::runtime_error("invalid query");
            }
        }
        catch (const std::exception& e) {
            std::cout << e.what();
            throw;
        }
    }
    my_mutex.unlock();
}

void DataBase::print() {
    for (auto& i : tables) {
        i.second.print();
    }
}

void DataBase::loadDataBaseFromFile(const std::string &path, DataBase& dataBase) {
    std::ifstream input_file(path);
    if (input_file.peek() != std::ifstream::traits_type::eof()) {
        json input_j;
        input_file >> input_j;
        if (!input_j.empty()) {
            from_json(input_j, dataBase);
            //dataBase.print();
        }
    }

    input_file.close();
}

void DataBase::storeDataBaseToFile(const std::string &path, DataBase& dataBase) {
    json output_j;
    dataBase.to_json(output_j);
    std::ofstream output_file("../data.json");
    if (output_file.is_open()) {
        output_file << output_j.dump(4);
    }
    output_file.flush();
    output_file.close();
}