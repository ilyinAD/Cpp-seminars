#ifndef CPP_SEMINARS_DATABASE_H
#define CPP_SEMINARS_DATABASE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <string.h>
#include <algorithm>
#include <math.h>
#include "utils/utils.h"
#include "domain/ColTable.h"
#include "domain/DataBaseType.h"

using namespace std;

class Table {
public:
    string name;
    vector<Col> columns;
    vector<vector<shared_ptr<DataBaseType>>> rows;
    Table(string name, map<string, string> m);

    Table(const Table& other) = default;

    void insert(map<string, string> m);

    Col getColByName(string colName);

    void update(map<string, string> m);

    void deleteRows(string cond);

    void print();
};

class DataBase {
public:
    string name;
    map<string, shared_ptr<Table>> tables;
    DataBase(string name) : name(name) {};

    void create(string tableName, map<string, string> m) {
        Table table = Table(tableName, m);
        tables[tableName] = make_shared<Table>(table);
    }
};


#endif //CPP_SEMINARS_DATABASE_H
