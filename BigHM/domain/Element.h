//
// Created by artem on 24.11.2024.
//

#ifndef CPP_SEMINARS_ELEMENT_H
#define CPP_SEMINARS_ELEMENT_H

#include "ColTable.h"

struct Element {
    Attributes attributes;
    string name;
    string value;
    Element() {
        attributes = Attributes();
        name = "";
        value = "";
    }
    Element(string name, string value) : name(name), value(value) {
        attributes = Attributes();
    };
    Element(string name, string value, Attributes attributes) : name(name), value(value), attributes(attributes) {};
};

#endif //CPP_SEMINARS_ELEMENT_H
