//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_DATABASETYPE_H
#define CPP_SEMINARS_DATABASETYPE_H
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;
class DataBaseType {
public:
    void *type;
    int sizeConstraint = 0;
    //DataBaseType() = default;

    //virtual DataBaseType& operator=(const DataBaseType& other) = 0;
    virtual void print() const = 0;
    virtual DataBaseType* operator+(const DataBaseType&) = 0;
    virtual DataBaseType* operator-(const DataBaseType&) = 0;
    virtual DataBaseType* operator*(const DataBaseType&) = 0;
    virtual DataBaseType* operator/(const DataBaseType&) = 0;
    virtual DataBaseType* operator%(const DataBaseType&) = 0;
    virtual DataBaseType* operator<(const DataBaseType&) = 0;
    virtual DataBaseType* operator>(const DataBaseType&) = 0;
    virtual DataBaseType* operator>=(const DataBaseType&) = 0;
    virtual DataBaseType* operator<=(const DataBaseType&) = 0;
    virtual DataBaseType* operator==(const DataBaseType&) = 0;
    virtual DataBaseType* operator!=(const DataBaseType&) = 0;
    virtual DataBaseType* operator&&(const DataBaseType&) = 0;
    virtual DataBaseType* operator||(const DataBaseType&) = 0;
    virtual DataBaseType* operator^(const DataBaseType&) = 0;
    virtual ~DataBaseType() = default;
};

class Bool : public DataBaseType {
public:
//    Bool(const DataBaseType& other) {
//        type = other.type;
//    }
    Bool(string strType) {
        if (strType == "True" || strType == "true") {
            type = new bool(true);
        } else if (strType == "False" || strType == "false") {
            type = new bool(false);
        } else {
            throw("Invalid bool type");
        }
    }

//    Bool& operator= (const DataBaseType& other) {
//        this->type = other.type;
//        return *this;
//    }

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
    DataBaseType* operator>= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) <= *static_cast<bool*>(other.type)));
    }
    DataBaseType* operator<= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<bool*>(type) <= *static_cast<bool*>(other.type)));
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

//    Int& operator= (const DataBaseType& other) {
//        this->type = other.type;
//        return *this;
//    }

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
    DataBaseType* operator>= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<int*>(type) >= *static_cast<int*>(other.type)));
    }
    DataBaseType* operator<= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<int*>(type) <= *static_cast<int*>(other.type)));
    }

};

class String : public DataBaseType {
public:
    String(string strType, int constraint) {
        sizeConstraint = constraint;
        if (strType.size() > constraint) {
            throw runtime_error("invalid size");
        }
        type = new string(strType);
    };
    String(string strType) {
//        if (strType.size() > sizeConstraint) {
//            throw runtime_error("Invalid size");
//        }
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
    DataBaseType* operator>= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) <= *static_cast<string*>(other.type)));
    }
    DataBaseType* operator<= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) <= *static_cast<string*>(other.type)));
    }
};

class Bytes: public DataBaseType {
public:

    Bytes (string strType, int constraint) {
        if (strType[0] == '0' && strType[1] == 'x') {
            if (strType.size() - 2 != constraint) {
                throw runtime_error("invalid size");
            }

            type = new string(strType.substr(2));
            //cout << *static_cast<string*>(type) << endl;
        } else {
            if (strType.size() != constraint) {
                throw runtime_error("invalid size");
            }
            type = new string(strType);
        }

        sizeConstraint = constraint;
    };

    Bytes (string strType) {
        if (strType[0] == '0' && strType[1] == 'x') {
            type = new string(strType.substr(2));
        } else {
            type = new string(strType);
        }
    };

    void print() const override {
        cout << *static_cast<string*>(type) << ' ';
    }

    DataBaseType* operator+ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator* (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator/ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator- (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator% (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bytes");
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
        throw invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator|| (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator^ (const DataBaseType& other) {
        throw invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator>= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) <= *static_cast<string*>(other.type)));
    }
    DataBaseType* operator<= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<string*>(type) <= *static_cast<string*>(other.type)));
    }
};
#endif //CPP_SEMINARS_DATABASETYPE_H
