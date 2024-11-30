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
    std::vector<std::vector<std::string>> rows = {{"0", "vasya", "0xdeadbeef", "true"}, {"1", "max", "0x00000000", "false"}};
    ASSERT_EQ(std::make_pair(rows.size(), rows[0].size()), std::make_pair(database.tables["users"].rows.size(), database.tables["users"].rows[0].size()));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (rows[i][j] != getStringByType(database.tables["users"].rows[i][j])) {
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
    rows = {{"0", "nissan", "false", "1"}, {"1", "toyota", "false", "0"}};
    ASSERT_EQ(std::make_pair(rows.size(), rows[0].size()), std::make_pair(database.tables["cars"].rows.size(), database.tables["cars"].rows[0].size()));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (rows[i][j] != getStringByType(database.tables["cars"].rows[i][j])) {
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

    rows = {{"0", "vasya", "0xdeadbeef", "true", "0", "nissan", "false", "1"}, {"0", "vasya", "0xdeadbeef", "true", "1", "toyota", "false", "0"},
                   {"1", "max", "0x00000000", "false", "0", "nissanCAR", "true", "1"}, {"1", "max", "0x00000000", "false", "1", "toyota", "false", "0"}};
    ASSERT_EQ(std::make_pair(rows.size(), rows[0].size()), std::make_pair(newtable.rows.size(), newtable.rows[0].size()));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (rows[i][j] != getStringByType(newtable.rows[i][j])) {
                FAIL() << i << ' ' << j << ' ' << rows[i][j] << ' ' << getStringByType(newtable.rows[i][j]) << std::endl;
            }
        }
    }

    Table selectTable = database.select("select cars.car_name, users.login, users.id, cars.person_id from cars join users on users.id = cars.person_id where true");
    columns = {Col("cars.car_name", Type(TypeName::String, 32), 0, Attributes(0, 0, 0)),
               Col("users.login", Type(TypeName::String, 32), 1, Attributes(1, 0, 0)),
               Col("users.id", Type(TypeName::Int, 1), 2, Attributes(0, 1, 1)),
               Col("cars.person_id", Type(TypeName::Int, 1), 3, Attributes(0, 0, 0))};
    ASSERT_EQ(columns, selectTable.columns);

    rows = {{"nissan", "max", "1", "1"}, {"toyota", "vasya", "0", "0"}};
    ASSERT_EQ(std::make_pair(rows.size(), rows[0].size()), std::make_pair(selectTable.rows.size(), selectTable.rows[0].size()));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (rows[i][j] != getStringByType(selectTable.rows[i][j])) {
                FAIL() << i << ' ' << j << ' ' << rows[i][j] << ' ' << getStringByType(selectTable.rows[i][j]) << std::endl;
            }
        }
    }
    database.deleteRows("delete users where (id + 1) / 3 + id < 1");
    columns = {Col("id", Type(TypeName::Int, 1), 0, Attributes(0, 1, 1)),
                                Col("login", Type(TypeName::String, 32), 1, Attributes(1, 0, 0)),
                                Col("password_hash", Type(TypeName::Bytes, 8), 2, Attributes(0, 0, 0)),
                                Col("is_admin", Type(TypeName::Bool, 1), 3, Attributes(0, 0, 0, "true"))};
    ASSERT_EQ(columns, database.tables["users"].columns);

    rows = {{"0", "vasya", "0xdeadbeef", "true"}};
    ASSERT_EQ(std::make_pair(rows.size(), rows[0].size()), std::make_pair(database.tables["users"].rows.size(), database.tables["users"].rows[0].size()));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if (rows[i][j] != getStringByType(newtable.rows[i][j])) {
                FAIL() << i << ' ' << j << ' ' << rows[i][j] << ' ' << getStringByType(newtable.rows[i][j]) << std::endl;
            }
        }
    }
}