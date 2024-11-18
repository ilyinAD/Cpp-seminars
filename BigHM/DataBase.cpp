//
// Created by artem on 16.11.2024.
//

#include "DataBase.h"
#include "ExprChecker.h"
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

void Table::update(const map<string, string>& m, const string& condExpr) {
    for (int i = 0; i < rows.size(); ++ i) {
        if (!ExprChecker::check(condExpr, rows[i], columns)) {
            continue;
        }
        for (auto& [colName, expr] : m) {
            Col col = this->getColByName(colName);
            if (col.name == "") {
                throw ("invalid column name");
            }
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
        joinColumns.push_back(i);
    }

    for (auto i : right.columns) {
        joinColumns.push_back(i);
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
            j->print();
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