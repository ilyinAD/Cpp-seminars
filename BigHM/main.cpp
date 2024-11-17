#include <bits/stdc++.h>
#include "DataBase.h"
using namespace std;

int main() {
    DataBase database = DataBase("my database");
    database.create("my table", {{"id", "int32"}, {"login", "string[32]"}, {"is_admin", "bool"}});
    shared_ptr<Table> table = database.tables["my table"];
    database.tables["my table"]->insert({{"login", "vasya"}, {"is_admin", "False"}, {"id", "123456"}});
    database.tables["my table"]->insert({{"login", "max"}, {"is_admin", "True"}, {"id", "223456"}});
    database.tables["my table"]->print();
    database.tables["my table"]->update({{"login", "login + xyz"}, {"is_admin", "true"}, {"id", "id + 111"}});
    database.tables["my table"]->print();
    database.tables["my table"]->deleteRows("");
    database.tables["my table"]->print();
}
