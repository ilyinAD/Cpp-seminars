//
// Created by artem on 17.11.2024.
//

#include "DataBaseType.h"
std::shared_ptr<DataBaseType> DataBaseType::from_json(const json &j) {
    if (j["type"] == "Bool") {
        return Bool::from_json(j);
    } else if (j["type"] == "Int") {
        return Int::from_json(j);
    } else if (j["type"] == "String") {
        return String::from_json(j);
    } else {
        return Bytes::from_json(j);
    }
}