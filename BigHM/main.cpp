#include <bits/stdc++.h>
#include "dataBase/DataBase.h"
#include "json/single_include/nlohmann/json.hpp"
using namespace std;

int main() {
    DataBase database = DataBase("my database");
    try {
//        std::ifstream input_file("../data.json");
//        if (input_file.peek() != std::ifstream::traits_type::eof()) {
//            json input_j;
//            input_file >> input_j;
//            if (!input_j.empty()) {
//                from_json(input_j, database);
//                database.print();
//            }
//        }
//
//        input_file.close();
        //DataBase::loadDataBaseFromFile("../data.json", database);
        //database.print();

        database.create("create table users ({key, autoincrement} id :\n"
                        "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:\n"
                        "bool)");
        //shared_ptr<Table> table = database.tables["my table"];
        //database.tables["users"].insert({{"login", "vasya"}, {"is_admin", "False"}, {"id", "9"}, {"password_hash", "0x10000000"}});
        //database.tables["users"].insert({{"login", "max"}, {"is_admin", "True"}, {"id", "12"}, {"password_hash", "0x00000000"}});
        database.insert("insert (login = \"vasya\", password_hash = 0xdeadbeefdeadbeef) to users");
        database.insert("insert (,\"max\", 0x0000000000000000, false) to users");




        //database.update("update users set login = login + Y where is_admin = true");
        database.tables["users"].print();
        database.create("create table cars ({key, autoincrement} id :\n"
                        "int32, car_name: string[32], is_truck:\n"
                        "bool = false, person_id: int32)");
        database.insert("insert (,\"nissan\",,1) to cars");
        database.insert("insert (,\"toyota\",,0) to cars");
        database.tables["cars"].print();

        database.create("create table phones ({key, autoincrement} id :\n"
                        "int32, name: string[32], is_new:\n"
                        "bool = false)");
        database.insert("insert (,\"apple\",true) to phones");
        database.insert("insert (,\"android\",) to phones");
        database.tables["phones"].print();
        Table newtable  = database.update("update users join cars on users.id = cars.id join phones on users.id = phones.id set cars.car_name = cars.car_name + \"CAR\" + \"CAR\" where true");
        newtable.print();
//        //Table table = database.tables["users"].join(database.tables["users"], database.tables["cars"], "users.id = cars.person_id", "new table");
//        //table.print();
//        //database.deleteRows("delete users where is_admin");
//        //database.tables["users"].print();
////    for (auto i = database.tables["my table"]->begin(); i != database.tables["my table"]->end(); i++) {
////        for (auto j : *i) {
////            j->print();
////        }
////        cout << endl;
////    }
////        database.tables["users"].update({{"password_hash", "0x12000000"}, {"login", "login + xyz + z"}, {"is_admin", "true"}, {"id", "(id + 2) * (111 - 100) + 10"}}, "id + 1");
////        database.tables["users"].print();
//        //database.tables["my table"]->deleteRows("id * 2 < 150");
//        //database.tables["my table"]->print();
//        //Table newTable = database.tables["my table"]->select({"id", "login"}, "is_admin", "me new table");
//        //newTable.print();
//        //database.create("my table 1", {{"company_id", "int32"}, {"company_name", "string[32]"}, {"is_rich", "bool"}});
//
//        //shared_ptr<Table> table = database.tables["my table"];
////        database.tables["my table 1"]->insert({{"company_name", "apple"}, {"is_rich", "False"}, {"company_id", "12"}});
////        database.tables["my table 1"]->insert({{"company_name", "android"}, {"is_rich", "True"}, {"company_id", "14"}});
////        database.tables["my table 1"]->print();
//
//        //Table joinTable = database.tables["my table 1"]->join(*database.tables["my table"], *database.tables["my table 1"], "id < 10 && login + company_name = vasyaapple", "new join table");
//        //joinTable.print();
//        DataBase::storeDataBaseToFile("../data.json", database);
    } catch(const exception& e) {
        database.tables["users"].print();
        cout << "Error: " << e.what() << endl;
    }
}