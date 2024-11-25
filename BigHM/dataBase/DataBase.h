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

class DataBase {
public:
    std::string name;
    std::map<std::string, Table> tables;
    DataBase(std::string name) : name(name) {};
    std::pair<std::vector<Element>, std::string> parseCreate(const std::string&);
//    void create(string tableName, vector<Element> m) {
//        Table table = Table(tableName, m);
//        tables[tableName] = make_shared<Table>(table);
//    }
    void create(std::string s);
    void insert(std::string s);
    void deleteRows(std::string s);
    Table select(std::string s);
    Table handleJoin(std::string s);
    Table update(std::string s);
};


#endif //CPP_SEMINARS_DATABASE_H
