//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_DATABASETYPE_H
#define CPP_SEMINARS_DATABASETYPE_H
#include <string>
#include <iostream>
using namespace std;
class DataBaseType {
public:
    void *type;
    DataBaseType() = default;
    virtual void print() const = 0;
    virtual DataBaseType* operator+(const DataBaseType&) = 0;
    virtual DataBaseType* operator-(const DataBaseType&) = 0;
    virtual DataBaseType* operator*(const DataBaseType&) = 0;
    virtual DataBaseType* operator/(const DataBaseType&) = 0;
    virtual DataBaseType* operator%(const DataBaseType&) = 0;
    virtual DataBaseType* operator<(const DataBaseType&) = 0;
    virtual DataBaseType* operator>(const DataBaseType&) = 0;
    virtual DataBaseType* operator==(const DataBaseType&) = 0;
    virtual DataBaseType* operator!=(const DataBaseType&) = 0;
    virtual DataBaseType* operator&&(const DataBaseType&) = 0;
    virtual DataBaseType* operator||(const DataBaseType&) = 0;
    virtual DataBaseType* operator^(const DataBaseType&) = 0;
};

class Bool : public DataBaseType {
public:
    Bool(string strType) {
        if (strType == "True" || strType == "true") {
            type = new bool(true);
        } else if (strType == "False" || strType == "false") {
            type = new bool(false);
        } else {
            throw("Invalid bool type");
        }
    }

    Bool(bool val) {
        type = new bool(val);
    }
    void print() const override {
        cout << *(static_cast<bool*>(type)) << ' ';
    }
    DataBaseType* operator+ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator* (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator/ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator- (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator% (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator< (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) < *static_cast<bool*>(other.type)));
    }
    DataBaseType* operator> (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) > *static_cast<bool*>(other.type)));
    }
    DataBaseType* operator!= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) != *static_cast<bool*>(other.type)));
    }
    DataBaseType* operator== (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) == *static_cast<bool*>(other.type)));
    }
    DataBaseType* operator&& (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) && *static_cast<bool*>(other.type)));
    }
    DataBaseType* operator|| (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) || *static_cast<bool*>(other.type)));
    }
    DataBaseType* operator^ (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) ^ *static_cast<bool*>(other.type)));
    }
};

class Int : public DataBaseType {
public:
    Int(string strType) {
        try {
            type = new int(stoi(strType));
        } catch (const std::invalid_argument& e) {
            throw(e.what());
        } catch (const std::out_of_range& e) {
            throw(e.what());
        }
    }

    Int(int val) {
        type = new int(val);
    };

    void print() const override {
        cout << *(static_cast<int*>(type)) << ' ';
    }
    DataBaseType* operator+ (const DataBaseType& other) {
        return new Int(*static_cast<int*>(other.type) + *static_cast<int*>(type));
    }
    DataBaseType* operator- (const DataBaseType& other) {
        return new Int( *static_cast<int*>(type) - *static_cast<int*>(other.type));
    }
    DataBaseType* operator* (const DataBaseType& other) {
        return new Int( *static_cast<int*>(type) * *static_cast<int*>(other.type));
    }
    DataBaseType* operator% (const DataBaseType& other) {
        return new Int( *static_cast<int*>(type) % *static_cast<int*>(other.type));
    }
    DataBaseType* operator/ (const DataBaseType& other) {
        return new Int( *static_cast<int*>(type) / *static_cast<int*>(other.type));
    }
    DataBaseType* operator< (const DataBaseType& other) {
        return new Bool( bool(*static_cast<int*>(type) < *static_cast<int*>(other.type)));
    }
    DataBaseType* operator> (const DataBaseType& other) {
        return new Bool( bool(*static_cast<int*>(type) > *static_cast<int*>(other.type)));
    }
    DataBaseType* operator== (const DataBaseType& other) {
        return new Bool( bool(*static_cast<int*>(type) == *static_cast<int*>(other.type)));
    }
    DataBaseType* operator!= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<int*>(type) != *static_cast<int*>(other.type)));
    }
    DataBaseType* operator&& (const DataBaseType& other) {
        throw invalid_argument("wrong operation with int");
    }
    DataBaseType* operator|| (const DataBaseType& other) {
        throw invalid_argument("wrong operation with int");
    }
    DataBaseType* operator^ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with int");
    }
};

class String : public DataBaseType {
public:
    String(string strType) {
        type = new string(strType);
    };
    void print() const override {
        cout << *(static_cast<string*>(type)) << ' ';
    }

    DataBaseType* operator+ (const DataBaseType& other) {
        return new String( *static_cast<string*>(type) + *static_cast<string*>(other.type));
    }
    DataBaseType* operator* (const DataBaseType& other) {
        throw invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator/ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator- (const DataBaseType& other) {
        throw invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator% (const DataBaseType& other) {
        throw invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator< (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) < *static_cast<string*>(other.type)));
    }
    DataBaseType* operator> (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) > *static_cast<string*>(other.type)));
    }
    DataBaseType* operator== (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) == *static_cast<string*>(other.type)));
    }
    DataBaseType* operator!= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) != *static_cast<string*>(other.type)));
    }
    DataBaseType* operator&& (const DataBaseType& other) {
        throw invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator|| (const DataBaseType& other) {
        throw invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator^ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with strings");
    }
};


#endif //CPP_SEMINARS_DATABASETYPE_H
