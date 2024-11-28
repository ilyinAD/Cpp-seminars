//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_COLTABLE_H
#define CPP_SEMINARS_COLTABLE_H

#include <string>
#include "../../json/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
struct Attributes {
public:
    bool is_unique;
    bool is_autoincrement;
    bool is_key;
    std::string default_value;
    Attributes(bool is_unique, bool is_autoincrement, bool is_key, std::string default_value) :
    is_unique(is_unique), is_autoincrement(is_autoincrement), is_key(is_key), default_value(std::move(default_value)) {};
    Attributes(bool is_unique, bool is_autoincrement, bool is_key) :
            is_unique(is_unique), is_autoincrement(is_autoincrement), is_key(is_key) {};
    Attributes() {
        is_autoincrement = 0;
        is_key = 0;
        is_unique = 0;
    };
    bool operator==(const Attributes& other) const{
        return is_unique == other.is_unique && is_autoincrement == other.is_autoincrement && is_key == other.is_key && default_value == other.default_value;
    }
    void to_json(json& j) {
        j = {{"is_unique", is_unique},
             {"is_autoincrement", is_autoincrement},
             {"is_key", is_key},
             {"default_value", default_value}};
    }
    friend void from_json(const json& j, Attributes& attributes) {
        attributes.is_key = j["is_key"];
        attributes.is_autoincrement = j["is_autoincrement"];
        attributes.is_unique = j["is_unique"];
        attributes.default_value = j["default_value"];
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

    void to_json(json& j) const {
        j["size"] = size;
        j["name"] = name;
    }
    friend void from_json(const json& j, Type& type) {
        type.size = j["size"];
        type.name = j["name"];
    }
//    friend void from_json(json& j, Type& t) {
//        j["size"] = t.size;
//        j["name"] = t.name;
//    }
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
    void to_json(json& j) {
        j["name"] = name;
        j["idx"] = idx;
        json type_json;
        type.to_json(type_json);
        j["type"] = type_json;
        json attributes_json;
        attributes.to_json(attributes_json);
        j["attributes"] = attributes_json;
    }
    friend void from_json(const json& j, Col& col) {
        col.name = j["name"];
        from_json(j["type"], col.type);
        from_json(j["attributes"], col.attributes);
    }
};


#endif //CPP_SEMINARS_COLTABLE_H
