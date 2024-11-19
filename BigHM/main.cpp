#include <bits/stdc++.h>
#include "DataBase.h"
using namespace std;

int main() {
    
    try {
        DataBase database = DataBase("my database");
        database.create("my table", {{"id", "int32"}, {"login", "string[32]"}, {"is_admin", "bool"}});

        //shared_ptr<Table> table = database.tables["my table"];
        database.tables["my table"]->insert({{"login", "vasya"}, {"is_admin", "False"}, {"id", "9"}});
        database.tables["my table"]->insert({{"login", "max"}, {"is_admin", "True"}, {"id", "12"}});
        database.tables["my table"]->print();
//    for (auto i = database.tables["my table"]->begin(); i != database.tables["my table"]->end(); i++) {
//        for (auto j : *i) {
//            j->print();
//        }
//        cout << endl;
//    }
        //database.tables["my table"]->update({{"login", "login + xyz"}, {"is_admin", "true"}, {"id", "id + 111"}}, "id * 2 + 3 < 22");
        //database.tables["my table"]->print();
        //database.tables["my table"]->deleteRows("id * 2 < 150");
        //database.tables["my table"]->print();
        //Table newTable = database.tables["my table"]->select({"id", "login"}, "is_admin", "me new table");
        //newTable.print();
        database.create("my table 1", {{"company_id", "int32"}, {"company_name", "string[32]"}, {"is_rich", "bool"}});

        //shared_ptr<Table> table = database.tables["my table"];
        database.tables["my table 1"]->insert({{"company_name", "apple"}, {"is_rich", "False"}, {"company_id", "12"}});
        database.tables["my table 1"]->insert({{"company_name", "android"}, {"is_rich", "True"}, {"company_id", "14"}});
        database.tables["my table 1"]->print();

        Table joinTable = database.tables["my table 1"]->join(*database.tables["my table"], *database.tables["my table 1"], "id < 10 && login + company_name = vasyaapple", "new join table");
        joinTable.print();
    } catch(const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

}
