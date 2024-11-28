//
// Created by artem on 17.11.2024.
//

#ifndef CPP_SEMINARS_DATABASETYPE_H
#define CPP_SEMINARS_DATABASETYPE_H
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../../json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;
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
    virtual void to_json(json& j) const = 0;
    static std::shared_ptr<DataBaseType> from_json(const json& j);
};



class Bool : public DataBaseType {
public:
    explicit Bool(const std::string& strType) {
        if (strType == "True" || strType == "true") {
            type = new bool(true);
        } else if (strType == "False" || strType == "false") {
            type = new bool(false);
            bool b = *static_cast<bool*>(type);
        } else {
            throw("Invalid bool type");
        }
        int ok = 1;
    }

    explicit Bool(bool val) {
        type = new bool(val);
    }

    void to_json(json& j) const {
        j = json{{"type", "Bool"}, {"DataBaseType", *static_cast<bool*>(type)}};
    }

    static std::shared_ptr<DataBaseType> from_json(const json& j) {
        return std::make_shared<Bool>(Bool(j.at("DataBaseType").get<bool>()));
    }

    void print() const override {
        std::cout << *(static_cast<bool*>(type)) << ' ';
    }
    DataBaseType* operator+ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator* (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator/ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator- (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bool");
    }
    DataBaseType* operator% (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bool");
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
    Int(std::string strType) {
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

    void to_json(json& j) const {
        j = json{{"type", "Int"}, {"DataBaseType", *static_cast<int*>(type)}};
    }

    static std::shared_ptr<DataBaseType> from_json(const json& j) {
        return std::make_shared<Int>(Int(j.at("DataBaseType").get<int>()));
    }

    void print() const override {
        std::cout << *(static_cast<int*>(type)) << ' ';
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
        throw std::invalid_argument("wrong operation with int");
    }
    DataBaseType* operator|| (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with int");
    }
    DataBaseType* operator^ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with int");
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
    String(std::string strType, int constraint) {
        sizeConstraint = constraint;
        if (strType.size() > constraint) {
            throw std::runtime_error("invalid size");
        }
        type = new std::string(strType);
    };
    String(std::string strType) {
        type = new std::string(strType);
    };

    void to_json(json& j) const {
        j = json{{"type", "String"}, {"DataBaseType", *static_cast<std::string*>(type)}};
    }

    static std::shared_ptr<DataBaseType> from_json(const json& j) {
        return std::make_shared<String>(String(j.at("DataBaseType").get<std::string>()));
    }

    void print() const override {
        std::cout << *(static_cast<std::string*>(type)) << ' ';
    }

    DataBaseType* operator+ (const DataBaseType& other) {
        return new String( *static_cast<std::string*>(type) + *static_cast<std::string*>(other.type));
    }
    DataBaseType* operator* (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator/ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator- (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator% (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator< (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) < *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator> (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) > *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator== (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) == *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator!= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) != *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator&& (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator|| (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator^ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with strings");
    }
    DataBaseType* operator>= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) <= *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator<= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) <= *static_cast<std::string*>(other.type)));
    }
};

class Bytes: public DataBaseType {
public:

    Bytes (std::string strType, int constraint) {
        if (strType[0] == '0' && strType[1] == 'x') {
            if (strType.size() - 2 != constraint) {
                throw std::runtime_error("invalid size");
            }

            type = new std::string(strType.substr(2));
            //cout << *static_cast<string*>(type) << endl;
        } else {
            if (strType.size() != constraint) {
                throw std::runtime_error("invalid size");
            }
            type = new std::string(strType);
        }

        sizeConstraint = constraint;
    };

    Bytes (std::string strType) {
        if (strType[0] == '0' && strType[1] == 'x') {
            type = new std::string(strType.substr(2));
        } else {
            type = new std::string(strType);
        }
    };

    void to_json(json& j) const {
        j = json{{"type", "Bytes"}, {"DataBaseType", *static_cast<std::string*>(type)}};
    }

    static std::shared_ptr<DataBaseType> from_json(const json& j) {
        return std::make_shared<Bytes>(Bytes(j.at("DataBaseType").get<std::string>()));
    }

    void print() const override {
        std::cout << *static_cast<std::string*>(type) << ' ';
    }

    DataBaseType* operator+ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator* (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator/ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator- (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator% (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator< (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) < *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator> (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) > *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator== (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) == *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator!= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) != *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator&& (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator|| (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator^ (const DataBaseType& other) {
        throw std::invalid_argument("wrong operation with bytes");
    }
    DataBaseType* operator>= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) <= *static_cast<std::string*>(other.type)));
    }
    DataBaseType* operator<= (const DataBaseType& other) {
        return new Bool( bool(*static_cast<std::string*>(type) <= *static_cast<std::string*>(other.type)));
    }
};

#endif //CPP_SEMINARS_DATABASETYPE_H
