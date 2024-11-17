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
using namespace std;

vector<string> splitString(const string &, char);
shared_ptr<DataBaseType> getTypeByCol(Type, string);


#endif //CPP_SEMINARS_UTILS_H