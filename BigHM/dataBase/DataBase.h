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
#include "../table/Table.h"

using namespace std;



class DataBase {
public:
    string name;
    map<string, Table> tables;
    DataBase(string name) : name(name) {};
    pair<vector<Element>, string> parseCreate(const string&);
//    void create(string tableName, vector<Element> m) {
//        Table table = Table(tableName, m);
//        tables[tableName] = make_shared<Table>(table);
//    }
    void create(string s);
    void insert(string s);
    void deleteRows(string s);
    Table select(string s);
    Table handleJoin(string s);
    Table update(string s);
};


#endif //CPP_SEMINARS_DATABASE_H
