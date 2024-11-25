#include <bits/stdc++.h>
#include "dataBase/DataBase.h"
using namespace std;

int main() {
    //    while (true) {
//        string s;
//        cin >> s;
//        s = toLower(s);
//        s = deleteDoubleSpaces(s);
//        string command = getCommand(s, 0);
//        if (command == "create") {
//
//        }
//    }
//    vector<Element> elemets = parseCreate("users ({key; autoincrement} id : "
//                                          "int32, {unique} login: string[32], password_hash: bytes[8], is_admin: "
//                                          "bool = false)");
    DataBase database = DataBase("my database");
//    database.insert("insert (login = \"vasya\", password_hash = 0xdeadbeefdeadbeef) to users");
//    return 0;
    try {
        database.create("create table users ({key, autoincrement} id :\n"
                        "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:\n"
                        "bool = true)");
        //shared_ptr<Table> table = database.tables["my table"];
        //database.tables["users"].insert({{"login", "vasya"}, {"is_admin", "False"}, {"id", "9"}, {"password_hash", "0x10000000"}});
        //database.tables["users"].insert({{"login", "max"}, {"is_admin", "True"}, {"id", "12"}, {"password_hash", "0x00000000"}});
        database.insert("insert (login = \"vasya\", password_hash = 0xdeadbeef) to users");
        database.insert("insert (,\"max\", 0x00000000, false) to users");
        database.tables["users"].print();
        database.create("create table cars ({key, autoincrement} id :\n"
                        "int32, car_name: string[32], is_truck:\n"
                        "bool = false, person_id: int32)");
        database.insert("insert (,\"nissan\",,1) to cars");
        database.insert("insert (,\"toyota\",,0) to cars");
        database.tables["cars"].print();
        Table newtable  = database.update("update users join cars on true set cars.car_name = cars.car_name + CAR, cars.is_truck = true where (cars.id) * (1 + 2) < 3 && users.id = 1)");
        newtable.print();
        //Table table = database.tables["users"].join(database.tables["users"], database.tables["cars"], "users.id = cars.person_id", "new table");
        //table.print();
        //database.deleteRows("delete users where is_admin");
        //database.tables["users"].print();
//    for (auto i = database.tables["my table"]->begin(); i != database.tables["my table"]->end(); i++) {
//        for (auto j : *i) {
//            j->print();
//        }
//        cout << endl;
//    }
//        database.tables["users"].update({{"password_hash", "0x12000000"}, {"login", "login + xyz + z"}, {"is_admin", "true"}, {"id", "(id + 2) * (111 - 100) + 10"}}, "id + 1");
//        database.tables["users"].print();
        //database.tables["my table"]->deleteRows("id * 2 < 150");
        //database.tables["my table"]->print();
        //Table newTable = database.tables["my table"]->select({"id", "login"}, "is_admin", "me new table");
        //newTable.print();
        //database.create("my table 1", {{"company_id", "int32"}, {"company_name", "string[32]"}, {"is_rich", "bool"}});

        //shared_ptr<Table> table = database.tables["my table"];
//        database.tables["my table 1"]->insert({{"company_name", "apple"}, {"is_rich", "False"}, {"company_id", "12"}});
//        database.tables["my table 1"]->insert({{"company_name", "android"}, {"is_rich", "True"}, {"company_id", "14"}});
//        database.tables["my table 1"]->print();

        //Table joinTable = database.tables["my table 1"]->join(*database.tables["my table"], *database.tables["my table 1"], "id < 10 && login + company_name = vasyaapple", "new join table");
        //joinTable.print();
    } catch(const exception& e) {
        database.tables["users"].print();
        cout << "Error: " << e.what() << endl;
    }
}