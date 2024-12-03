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
#include <fstream>

class DataBase {
public:
    std::mutex my_mutex;
    std::string name;
    std::map<std::string, Table> tables;
    DataBase(std::string name) : name(name) {};
    std::pair<std::vector<Element>, std::string> parseCreate(const std::string&);

    void create(std::string s);

    void insert(std::string s);
    std::pair<std::string, std::map<std::string, std::string>> parseInsert(std::string s);

    void deleteRows(std::string s);

    Table select(std::string s);

    Table handleJoin(std::string s, std::vector<std::string>& v, int& idx);

    Table update(std::string s);

    void execute(std::string s);

    void to_json(json& j) {
        j["name"] = name;
        json tables_json;
        for (auto& i : tables) {
            json table_json;
            i.second.to_json(table_json);
            tables_json.push_back(table_json);
        }
        j["tables"] = tables_json;
    }
    friend void from_json(json& j, DataBase& d) {
        d.name = j["name"];
        for (auto& i : j["tables"]) {
            Table table;
            from_json(i, table);
            d.tables[table.name] = table;
        }
    }
    void print();
    static void loadDataBaseFromFile(const std::string& path, DataBase& dataBase);
    static void storeDataBaseToFile(const std::string& path, DataBase& dataBase);
};


#endif //CPP_SEMINARS_DATABASE_H
