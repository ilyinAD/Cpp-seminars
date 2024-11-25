//
// Created by artem on 24.11.2024.
//

#ifndef CPP_SEMINARS_TABLE_H
#define CPP_SEMINARS_TABLE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <string.h>
#include <algorithm>
#include <math.h>
#include "../utils/utils.h"
#include "../domain/ColTable.h"
#include "../domain/DataBaseType.h"
#include "../domain/Element.h"
#include "../conditionParser/ExprChecker.h"

class Table {
public:
    std::string name;
    std::vector<Col> columns;
    std::vector<std::vector<std::shared_ptr<DataBaseType>>> rows;
    Table(std::string name, std::vector<Element> m);

    Table(const Table& other) {
        name = other.name;
        columns = other.columns;
        rows = other.rows;
    };

    Table() {};

    Table(std::string name, std::vector<Col> newColumns) : name(name) {
        int idx = 0;
        for (auto i : newColumns) {
            Col col = i;
            col.idx = idx;
            columns.push_back(col);
            ++idx;
        }
    };

    class Iterator {
        typename std::vector<std::vector<std::shared_ptr<DataBaseType>>>::iterator it;
    public:
        Iterator(typename std::vector<std::vector<std::shared_ptr<DataBaseType>>>::iterator it): it(it) {};
        Iterator& operator++() {
            it++;
            return *this;
        }
        Iterator& operator++(int) {
            Iterator it1 = *this;
            it++;
            return it1;
        }

        std::vector<std::shared_ptr<DataBaseType>>& operator*() {
            return *it;
        }

        bool operator!=(const Iterator& other) {
            return it != other.it;
        }

        bool operator==(const Iterator& other) {
            return it == other.it;
        }
    };

    Iterator begin();

    Iterator end();

    void insert(const std::map<std::string, std::string>& m);

    Col getColByName(std::string colName);

    void update(const std::map<std::string, std::string>& m, const std::string&);

    void deleteRows(std::string cond);

    Table select(std::vector<std::string>namesCol, std::string cond, std::string);

    void print();

    Table join(const Table&, const Table&, std::string, std::string);

    bool checkOnUnique(int, int);
};


#endif //CPP_SEMINARS_TABLE_H
