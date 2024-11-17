//
// Created by artem on 16.11.2024.
//

#include "DataBase.h"
#include "./utils/utils.h"
using namespace std;

Table::Table(string name, map<string, string> m): name(name) {
        int idx = 0;
        for (auto i : m) {
            string type = i.second;
            string colName = i.first;
            if (type == "int32") {
                columns.push_back(Col(colName, Type(TypeName::Int, 1), idx));
            } else if (type == "bool") {
                columns.push_back(Col(colName, Type(TypeName::Bool, 1), idx));
            } else {
                size_t start = type.find('[');
                size_t end = type.find(']');

                string numberStr = type.substr(start + 1, end - start - 1);
                int size = stoi(numberStr);
                columns.push_back(Col(colName, Type(TypeName::String, size), idx));
            }
            ++idx;
        }
}

//shared_ptr<DataBaseType> Table::getTypeByCol(Type col, string value) {
//    if (col.name == TypeName::Int) {
//        return make_shared<Int>(Int(value));
//    } else if (col.name == TypeName::Bool) {
//        return make_shared<Bool>(Bool(value));
//    } else {
//        if (value.size() > col.size) {
//            throw("Invalid size of string");
//        }
//        return make_shared<String>(String(value));
//    }
//}

void Table::insert(map<string, string> m) {
    int idx = rows.size();
    rows.push_back({});
    for (int i = 0; i < columns.size(); ++i) {
        rows[idx].push_back(nullptr);
    }
    for (auto &[colName, value] : m) {
        for (int i = 0; i < columns.size(); ++i) {
            if (columns[i].name != colName) {
                continue;
            }
            shared_ptr<DataBaseType> type;
            try {
                type = getTypeByCol(columns[i].type, value);
                rows[idx][i] = std::move(type);
            } catch (string err) {
                cerr << err << endl;
                return;
            }

        }
    }
}

Col Table::getColByName(string colName) {
    for (auto i : columns) {
        if (i.name == colName) {
            return  i;
        }
    }
    return Col();
}

//    bool isRowUnderCondition(string condition, vector<shared_ptr<DataBaseType>>& row) {
//        std::regex condition_regex(R"((\"[a-zA-Z_][a-zA-Z0-9_]*\")\s*(==|<|>)\s*([^&]*))");
//        std::smatch match;
//
//        std::istringstream condition_stream(condition);
//        bool result = true;
//
//        while (std::getline(condition_stream, condition, '&')) {
//            condition = condition.substr(condition.find_first_not_of(" ")); // Отрезаем пробелы с начала
//            if (std::regex_search(condition, match, condition_regex)) {
//                std::string columnName = match[1];
//                std::string operation = match[2];
//                std::string value = match[3];
//
//                shared_ptr<DataBaseType> val = row[this->getColByName(columnName).idx];
//
//
//                if (operation == "==") {
//                    result = (val.get()->type == value);
//                } else if (operation == "<") {
//                    result = (val < value);
//                } else if (operation == ">") {
//                    result = (val > value);
//                }
//            }
//        }
//
//        return result;
//    }

void Table::update(map<string, string> m) {
    for (auto& [colName, expr] : m) {
        Col col = this->getColByName(colName);
        if (col.name == "") {
            throw ("invalid column name");
        }

        for (int i = 0; i < rows.size(); ++ i) {
            if (expr.find(colName) == string::npos) {
                this->rows[i][col.idx] = getTypeByCol(col.type, expr);
            } else {
                vector<string> v = splitString(expr, ' ');
                if (v[1] == "+") {
                    shared_ptr<DataBaseType> val = getTypeByCol(col.type, v[2]);
                    DataBaseType* d = ((*this->rows[i][col.idx].get()) + (*val.get()));
                    shared_ptr<DataBaseType> sharedPtr(d);
                    this->rows[i][col.idx] = sharedPtr;
                }
            }
        }
    }
}

void Table::deleteRows(string cond) {
    for (auto it = rows.begin(); it != rows.end();) {
        it = rows.erase(it);
    }
}

void Table::print() {
    cout << name << endl;
    for (auto i : columns) {
        cout << i.name << ' ' << static_cast<int>(i.type.name) << ' ' << i.type.size << endl;
    }
    for (const auto& i : rows) {
        for (const auto& j : i) {
            j->print();
        }
        cout << endl;
    }
    cout << endl;
}