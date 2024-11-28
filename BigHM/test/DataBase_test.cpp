//
// Created by artem on 26.11.2024.
//

#include "gtest/gtest.h"
#include "dataBase/DataBase.h"
std::string False = "false";
std::string True = "true";

TEST(DataBase, Queries) {
    DataBase database = DataBase("my database");
    database.create("create table users ({key, autoincrement} id :\n"
                    "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:\n"
                    "bool = true)");
    std::vector<Col> columns = {Col("id", Type(TypeName::Int, 1), 0, Attributes(0, 1, 1)),
                                Col("login", Type(TypeName::String, 32), 1, Attributes(1, 0, 0)),
                                Col("password_hash", Type(TypeName::Bytes, 8), 2, Attributes(0, 0, 0)),
                                Col("is_admin", Type(TypeName::Bool, 1), 3, Attributes(0, 0, 0, "true"))};
    ASSERT_EQ(columns, database.tables["users"].columns);

    database.insert("insert (login = \"vasya\", password_hash = 0xdeadbeef) to users");
    database.insert("insert (,\"max\", 0x00000000, false) to users");
    std::vector<std::vector<std::shared_ptr<DataBaseType>>> rows;
    rows.push_back({});
    rows[0].emplace_back(
            std::make_shared<Int>(Int("0")));
    rows[0].emplace_back(
            std::make_shared<String>(String("vasya")));
    rows[0].emplace_back(
            std::make_shared<Bytes>(Bytes("0xdeadbeef")));
    rows[0].emplace_back(
            std::make_shared<Bool>(Bool(True)));
    rows.push_back({});
    rows[1].emplace_back(
            std::make_shared<Int>(Int("1")));
    rows[1].emplace_back(
            std::make_shared<String>(String("max")));
    rows[1].emplace_back(
            std::make_shared<Bytes>(Bytes("0x00000000")));
    rows[1].emplace_back(std::make_shared<Bool>(Bool(False)));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (*static_cast<bool*>(((*rows[i][j]) != (*database.tables["users"].rows[i][j]))->type)) {
                FAIL() << i << j << std::endl;
            }
        }
    }

    database.create("create table cars ({key, autoincrement} id :\n"
                    "int32, car_name: string[32], is_truck:\n"
                    "bool = false, person_id: int32)");
    columns = {Col("id", Type(TypeName::Int, 1), 0, Attributes(0, 1, 1)),
                                Col("car_name", Type(TypeName::String, 32), 1, Attributes(0, 0, 0)),
                                Col("is_truck", Type(TypeName::Bool, 1), 2, Attributes(0, 0, 0, "false")),
                                Col("person_id", Type(TypeName::Int, 1), 3, Attributes(0, 0, 0))};
    ASSERT_EQ(columns, database.tables["cars"].columns);
    database.insert("insert (,\"nissan\",,1) to cars");
    database.insert("insert (,\"toyota\",,0) to cars");
    rows.clear();
    rows.push_back({});
    rows[0].emplace_back(
            std::make_shared<Int>(Int("0")));
    rows[0].emplace_back(
            std::make_shared<String>(String("nissan")));
    rows[0].emplace_back(
            std::make_shared<Bool>(Bool(False)));
    rows[0].emplace_back(
            std::make_shared<Bool>(Bool(True)));
    rows.push_back({});
    rows[1].emplace_back(
            std::make_shared<Int>(Int("1")));
    rows[1].emplace_back(
            std::make_shared<String>(String("toyota")));
    rows[1].emplace_back(
            std::make_shared<Bool>(Bool(False)));
    rows[1].emplace_back(
            std::make_shared<Bool>(Bool(False)));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (*static_cast<bool*>(((*rows[i][j]) != (*database.tables["cars"].rows[i][j]))->type)) {
                FAIL() << i << j << std::endl;
            }
        }
    }
    Table newtable  = database.update("update users join cars on true set cars.car_name = cars.car_name + \"CAR\", cars.is_truck = true where (cars.id) * (1 + 2) < 3 && users.id = 1 ");
    columns = {Col("users.id", Type(TypeName::Int, 1), 0, Attributes(0, 1, 1)),
               Col("users.login", Type(TypeName::String, 32), 1, Attributes(1, 0, 0)),
               Col("users.password_hash", Type(TypeName::Bytes, 8), 2, Attributes(0, 0, 0)),
               Col("users.is_admin", Type(TypeName::Bool, 1), 3, Attributes(0, 0, 0, "true")),
               Col("cars.id", Type(TypeName::Int, 1), 4, Attributes(0, 1, 1)),
               Col("cars.car_name", Type(TypeName::String, 32), 5, Attributes(0, 0, 0)),
               Col("cars.is_truck", Type(TypeName::Bool, 1), 6, Attributes(0, 0, 0, "false")),
               Col("cars.person_id", Type(TypeName::Int, 1), 7, Attributes(0, 0, 0))};

    ASSERT_EQ(columns, newtable.columns);

}