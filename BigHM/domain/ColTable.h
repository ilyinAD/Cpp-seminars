//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_COLTABLE_H
#define CPP_SEMINARS_COLTABLE_H

#include <string>
using namespace std;

enum class TypeName {
    Int,
    Bool,
    String
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
    string name;
    Type type;
    int idx;
    Col(string name, Type type, int idx) : name(name), type(type), idx(idx) {};
    Col() {
        name = "";
        idx = 0;
    };
};


#endif //CPP_SEMINARS_COLTABLE_H
