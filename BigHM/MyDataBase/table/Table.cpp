//
// Created by artem on 24.11.2024.
//

#include "Table.h"

using namespace std;

Table::Table(string name, vector<Element> m): name(name) {
    int idx = 0;
    for (auto& element : m) {
        string type = element.value;
        string colName = element.name;
        if (type == "int32") {
            columns.push_back(Col(colName, Type(TypeName::Int, 1), idx, element.attributes));
        } else if (type == "bool") {
            columns.push_back(Col(colName, Type(TypeName::Bool, 1), idx, element.attributes));
        } else {
            size_t start = type.find('[');
            size_t end = type.find(']');
            string s = type.substr(0, start);
            string numberStr = type.substr(start + 1, end - start - 1);
            int size = stoi(numberStr);
            if (s == "string") {
                columns.push_back(Col(colName, Type(TypeName::String, size), idx, element.attributes));
            } else {
                columns.push_back(Col(colName, Type(TypeName::Bytes, size), idx, element.attributes));
            }
        }
        ++idx;
    }
}

//проверяет только что нужная строка имеет неуникальные значения в колонке с индексом idx
bool Table::checkOnUnique(int colIdx, int rowIdx) {
    if (rows[rowIdx][colIdx] == nullptr) {
        return false;
    }
    for (int i = 0; i < rows.size(); ++i) {
        if (i == rowIdx) {
            continue;
        }
        if ((*rows[i][colIdx]).type == (*rows[rowIdx][colIdx]).type) {
            rows.pop_back();
            return true;
        }
    }

    return false;
}

void Table::insert(const map<string, string>& m) {
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
            } catch (const std::exception& e) {
                cout << e.what() << endl;
                rows.pop_back();
                throw;
            }
        }
    }
    for (int i = 0; i < rows[idx].size(); ++i) {
        if (rows[idx][i] == nullptr) {
            try {
                if (columns[i].attributes.is_autoincrement) {
                    rows[idx][i] = getTypeByCol(columns[i].type, to_string(rows.size() - 1));
                } else if (columns[i].attributes.default_value != "") {
                    rows[idx][i] = getTypeByCol(columns[i].type, columns[i].attributes.default_value);
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error" << e.what() << endl;
                rows.pop_back();
                throw;
            }
        }

        if ((columns[i].attributes.is_unique || columns[i].attributes.is_key) && checkOnUnique(i, rows.size() - 1)) {
            rows.pop_back();
            throw invalid_argument("not unique value");
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

bool checkType(shared_ptr<DataBaseType> type, Col needType) {
    if (needType.type.name == TypeName::Int) {
        if (!dynamic_pointer_cast<Int>(type)) {
            throw runtime_error("invalid type");
        }
    }
    if (needType.type.name == TypeName::Bool) {
        if (!dynamic_pointer_cast<Bool>(type)) {
            throw runtime_error("invalid type");
        }
    }
    if (needType.type.name == TypeName::String) {
        if (needType.type.size < static_cast<string*>(type->type)->size()) {
            throw runtime_error("invalid size");
        }
        if (!dynamic_pointer_cast<String>(type)) {
            throw runtime_error("invalid type");
        }
    }
    if (needType.type.name == TypeName::Bytes) {
        if (needType.type.size != static_cast<string*>(type->type)->size()) {
            throw runtime_error("invalid size");
        }
        if (!dynamic_pointer_cast<Bytes>(type)) {
            throw runtime_error("invalid type");
        }
    }
    return true;
}

void Table::update(const map<string, string>& m, const string& condExpr) {
    for (int i = 0; i < rows.size(); ++ i) {
        if (!ExprChecker::check(condExpr, rows[i], columns)) {
            continue;
        }
        vector<shared_ptr<DataBaseType>> copyRow = rows[i];
        for (auto& [colName, expr] : m) {
            Col col = this->getColByName(colName);
            if (col.name == "") {
                rows[i] = copyRow;
                throw runtime_error("invalid column name");
            }
            shared_ptr<DataBaseType> res = ExprChecker::getValFromExpr(expr, rows[i], columns);
            try {
                checkType(res, col);
            } catch (const std::exception& e) {
                rows[i] = copyRow;
                cout << "Error" << ' ' << e.what()  << endl;
                throw;
            }
            rows[i][col.idx] = res;
            if ((columns[i].attributes.is_unique || columns[i].attributes.is_key) && checkOnUnique(col.idx, i)) {
                throw invalid_argument("not unique value in update");
            }
        }
    }
}

void Table::deleteRows(string cond) {
    for (auto it = rows.begin(); it != rows.end();) {
        if (!ExprChecker::check(cond, *it, columns)) {
            ++it;
            continue;
        }
        it = rows.erase(it);
    }
}

Table Table::select(vector<std::string> namesCol, std::string cond, string tableName = "") {
    vector<Col> newColumns;
    for (int j = 0; j < namesCol.size(); ++j) {
        Col col = getColByName(namesCol[j]);
        newColumns.push_back(col);
    }
    Table newTable = Table(tableName != "" ? tableName : name + cond, newColumns);
    for (int i = 0; i < rows.size(); ++i) {
        if (!ExprChecker::check(cond, rows[i], columns)) {
            continue;
        }
        newTable.rows.push_back({});
        for (int j = 0; j < namesCol.size(); ++j) {
            Col col = getColByName(namesCol[j]);
            newTable.rows[newTable.rows.size() - 1].push_back(rows[i][col.idx]);
        }
    }

    return newTable;
}

Table Table::join(const Table& left, const Table& right, string cond, string tableName) {
    vector<Col> joinColumns;
    for (auto i : left.columns) {
        Col col = i;
        if (col.name.find('.') >= col.name.size()) {
            col.name = left.name + '.' + col.name;
        }
        //col.name = left.name + '.' + col.name;
        joinColumns.push_back(col);
    }

    for (auto i : right.columns) {
        Col col = i;
        col.name = right.name + '.' + col.name;
        joinColumns.push_back(col);
    }

    Table joinTable = Table(tableName, joinColumns);
    for (int i = 0; i < left.rows.size(); ++i) {
        for (int j = 0; j < right.rows.size(); ++j) {
            vector<shared_ptr<DataBaseType>> joinRow;
            for (int k = 0; k < left.rows[i].size(); ++k) {
                joinRow.push_back(left.rows[i][k]);
            }
            for (int k = 0; k < right.rows[j].size(); ++k) {
                joinRow.push_back(right.rows[j][k]);
            }
            if (!ExprChecker::check(cond, joinRow, joinTable.columns)) {
                continue;
            }

            joinTable.rows.push_back(joinRow);
        }
    }

    return joinTable;
}

void Table::print() {
    cout << name << endl;
    for (auto i : columns) {
        cout << i.name << ' ' << static_cast<int>(i.type.name) << ' ' << i.type.size << endl;
    }
    for (const auto& i : rows) {
        for (const auto& j : i) {
            if (j == nullptr) {
                cout << "null ";
            } else {
                j->print();
            }
        }
        cout << endl;
    }
    cout << endl;
}

Table::Iterator Table::begin() {
    return Iterator(rows.begin());
}

Table::Iterator Table::end() {
    return Iterator(rows.end());
}

void Table::to_json(json &j) {
    j["name"] = name;
    json row_json;
    for (auto& i : rows) {
        json vec_json;
        for (auto& el : i) {
            json el_json;
            if (el) {
                el->to_json(el_json);
            } else {
                el_json = nullptr;
            }
            vec_json.push_back(el_json);
        }
        row_json.push_back(vec_json);
    }
    j["row"] = row_json;
    json columns_json;
    for (auto& i : columns) {
        json el_json;
        i.to_json(el_json);
        columns_json.push_back(el_json);
    }
    j["col"] = columns_json;
}

void from_json(json& j, Table& table) {
    table.name = j["name"];
    for (auto i : j["row"]) {
        vector<shared_ptr<DataBaseType>> v;
        for (auto el : i) {
            v.push_back(DataBaseType::from_json(el));
        }
        table.rows.push_back(v);
    }
    for (auto i : j["col"]) {
        Col col;
        from_json(i, col);
        table.columns.push_back(col);
    }
}