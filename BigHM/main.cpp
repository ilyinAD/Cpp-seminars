#include <bits/stdc++.h>
#include "DataBase.h"
using namespace std;

string toLower(const string& s) {
    string ans;
    for (auto i : s) {
        ans += tolower(i);
    }
    return ans;
}

string deleteDoubleSpaces(const string& s) {
    string ans;
    char prev = '_';
    for (auto i : s) {
        if (i == ' ' && prev == ' ') {
            continue;
        }
        prev = i;
        ans += i;
    }
}

string getCommand(const string& s, int idx) {
    string ans;
    for (int i = idx; i < s.size(); ++i) {
        if (s[idx] == ' ') {
            break;
        }
        ans += s[i];
    }
    return ans;
}

string deleteSpaces(const string& s) {
    string ans;
    for (auto i : s) {
        if (i != ' ') {
            ans += i;
        }
    }

    return ans;
}

vector<Element> parseCreate(const string& schema) {
    vector<Element> elements;
    string table_name, fields_section;
    size_t start_pos = schema.find('(');
    size_t end_pos = schema.find(')');
    table_name = schema.substr(0, start_pos - 1);
    fields_section = schema.substr(start_pos + 1, end_pos - start_pos - 1);

    istringstream stream(fields_section);
    string field;

    while (getline(stream, field, ',')) {
        Element element;
        Attributes attributes;
        size_t brace_start = field.find('{');
        size_t brace_end = field.find('}');
        if (brace_start != string::npos && brace_end != string::npos) {
            string attr_section = field.substr(brace_start + 1, brace_end - brace_start - 1);
            if (attr_section.find("key") != string::npos) attributes.is_key = true;
            if (attr_section.find("autoincrement") != string::npos) attributes.is_autoincrement = true;
            if (attr_section.find("unique") != string::npos) attributes.is_unique = true;
            field = field.substr(brace_end + 2); // Убираем атрибуты из строки
        }

        size_t colon_pos = field.find(':');
        string name = field.substr(0, colon_pos);
        string value = field.substr(colon_pos + 2);

        size_t equals_pos = value.find('=');
        if (equals_pos != string::npos) {
            attributes.default_value = value.substr(equals_pos + 2);
            value = value.substr(0, equals_pos - 1);
        }

        element.name = name;
        element.value = value;
        element.attributes = attributes;
        elements.push_back(element);
    }
    for (auto& i : elements) {
        i.name = deleteSpaces(i.name);
        i.value = deleteSpaces(i.value);
        i.attributes.default_value = deleteSpaces(i.attributes.default_value);
    }
    return elements;
}

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
    vector<Element> elemets = parseSchema("users ({key; autoincrement} id : "
                                          "int32, {unique} login: string[32], password_hash: bytes[8], is_admin: "
                                          "bool = false)");
    try {
        DataBase database = DataBase("my database");
        database.create("my table", {Element("id", "int32"), Element("login", "string[32]"), Element{"is_admin", "bool"}});


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
        database.tables["my table"]->update({{"login", "login + xyz + z"}, {"is_admin", "true"}, {"id", "id * 2 + 111"}}, "id + 1");
        database.tables["my table"]->print();
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
        cout << "Error: " << e.what() << endl;
    }

}
