cmake_minimum_required(VERSION 3.13)
project(BigHM)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
        dataBase/DataBase.h
        utils/utils.h
        utils/utils.cpp
        dataBase/DataBase.cpp
        conditionParser/ExprChecker.h
        conditionParser/ExprChecker.cpp
        domain/DataBaseType.h
        domain/ColTable.h
        domain/DataBaseType.h
        domain/DataBaseType.cpp
        table/Table.h
        domain/Element.h
        table/Table.cpp
)

add_executable(BigHM ${SOURCE_FILES})