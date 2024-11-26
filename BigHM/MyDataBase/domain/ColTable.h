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
    is_unique(is_unique), is_autoincrement(is_autoincrement), is_key(is_key), default_value(std::move(default_value)) {
        is_unique |= is_key;
    };
    Attributes(bool is_unique, bool is_autoincrement, bool is_key) :
            is_unique(is_unique), is_autoincrement(is_autoincrement), is_key(is_key) {
        is_unique |= is_key;
    };
    Attributes() {
        is_autoincrement = 0;
        is_key = 0;
        is_unique = 0;
    };
    bool operator==(const Attributes& other) const{
        return is_unique == other.is_unique && is_autoincrement == other.is_autoincrement && is_key == other.is_key && default_value == other.default_value;
    }
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
    bool operator==(const Type& other) const {
        return size == other.size && name == other.name;
    }
};

class Col {
public:
    Attributes attributes;
    std::string name;
    Type type;
    int idx;
    Col(std::string name, Type type, int idx, Attributes attributes) : name(std::move(name)), type(type), idx(idx), attributes(std::move(attributes)){};
    Col() {
        name = "";
        idx = 0;
    };
    bool operator==(const Col& other) const {
        return name == other.name && type == other.type && idx == other.idx && attributes == other.attributes;
    }
};


#endif //CPP_SEMINARS_COLTABLE_H
