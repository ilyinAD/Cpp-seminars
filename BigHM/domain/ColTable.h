//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_COLTABLE_H
#define CPP_SEMINARS_COLTABLE_H

#include <string>

struct Attributes {
public:
    bool is_unique;
    bool is_autoincrement;
    bool is_key;
    std::string default_value;
    Attributes(bool is_unique, bool is_autoincrement, bool is_key, std::string default_value) :
    is_unique(is_unique), is_autoincrement(is_autoincrement), is_key(is_key), default_value(default_value) {};
    Attributes() {
        is_autoincrement = 0;
        is_key = 0;
        is_unique = 0;
    };
};


enum class TypeName {
    Int,
    Bool,
    String,
    Bytes,
};

class Type {
public:

    TypeName name;
    int size;
    Type(TypeName typeName, int size) : name(typeName), size(size) {};
    Type() {
        size = 0;
    }
};

class Col {
public:
    Attributes attributes;
    std::string name;
    Type type;
    int idx;
    Col(std::string name, Type type, int idx, Attributes attributes) : name(name), type(type), idx(idx), attributes(attributes){};
    Col() {
        name = "";
        idx = 0;
    };
};


#endif //CPP_SEMINARS_COLTABLE_H
