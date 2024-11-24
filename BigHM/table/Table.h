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
using namespace std;

class Table {
public:
    string name;
    vector<Col> columns;
    vector<vector<shared_ptr<DataBaseType>>> rows;
    Table(string name, vector<Element> m);

    Table(const Table& other) {
        name = other.name;
        columns = other.columns;
        rows = other.rows;
    };

    Table() {};

    Table(string name, vector<Col> newColumns) : name(name) {
        int idx = 0;
        for (auto i : newColumns) {
            Col col = i;
            col.idx = idx;
            columns.push_back(col);
            ++idx;
        }
    };

    class Iterator {
        typename vector<vector<shared_ptr<DataBaseType>>>::iterator it;
    public:
        Iterator(typename vector<vector<shared_ptr<DataBaseType>>>::iterator it): it(it) {};
        Iterator& operator++() {
            it++;
            return *this;
        }
        Iterator& operator++(int) {
            Iterator it1 = *this;
            it++;
            return it1;
        }

        vector<shared_ptr<DataBaseType>>& operator*() {
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

    void insert(const map<string, string>& m);

    Col getColByName(string colName);

    void update(const map<string, string>& m, const string&);

    void deleteRows(string cond);

    Table select(vector<string>namesCol, string cond, string);

    void print();

    Table join(const Table&, const Table&, string, string);

    bool checkOnUnique(int, int);
};


#endif //CPP_SEMINARS_TABLE_H
