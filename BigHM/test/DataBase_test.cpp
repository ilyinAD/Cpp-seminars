//
// Created by artem on 26.11.2024.
//

#include "gtest/gtest.h"
#include "dataBase/DataBase.h"
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

//    database.insert("insert (login = \"vasya\", password_hash = 0xdeadbeef) to users");
//    database.insert("insert (,\"max\", 0x00000000, false) to users");
//    std::vector<std::vector<std::shared_ptr<DataBaseType>>> rows(2);
//    rows[0].insert(rows[0].cbegin(),{
//        std::make_shared<Int>(Int("0")),
//        std::make_shared<String>(String("vasya")),
//        std::make_shared<Bytes>(Bytes("0xdeadbeef")),
//        std::make_shared<Bool>(Bool("true"))});
//    rows[1].insert(rows[1].cbegin(),{
//            std::make_shared<Int>(Int("1")),
//            std::make_shared<String>(String("max")),
//            std::make_shared<Bytes>(Bytes("0x00000000")),
//            std::make_shared<Bool>(Bool("false"))});
//    for (int i = 0; i < rows.size(); ++i) {
//        for (int j = 0; j < rows[i].size(); ++j) {
//            if (rows[i][j].get() != database.tables["users"].rows[i][j].get()) {
//                FAIL() << i << j << std::endl;
//            }
//        }
    }

//    database.create("create table cars ({key, autoincrement} id :\n"
//                    "int32, car_name: string[32], is_truck:\n"
//                    "bool = false, person_id: int32)");
//    database.insert("insert (,\"nissan\",,1) to cars");
//    database.insert("insert (,\"toyota\",,0) to cars");
//    Table newtable  = database.update("update users join cars on true set cars.car_name = cars.car_name + CAR, cars.is_truck = true where (cars.id) * (1 + 2) < 3 && users.id = 1)");
}