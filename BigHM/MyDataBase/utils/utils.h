//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_UTILS_H
#define CPP_SEMINARS_UTILS_H
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include "../domain/ColTable.h"
#include "../domain/DataBaseType.h"

std::vector<std::string> splitString(const std::string &, char);
std::shared_ptr<DataBaseType> getTypeByCol(Type, std::string);
std::string toLower(const std::string& s);
std::string deleteDoubleSpaces(const std::string& s);
std::string getCommand(const std::string& s, int idx);
std::string deleteSpaces(const std::string& s);
std::string deleteCornerSpaces(const std::string& s);
std::string getStringByType(std::shared_ptr<DataBaseType> val);

#endif //CPP_SEMINARS_UTILS_H
