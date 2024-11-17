cmake_minimum_required(VERSION 3.13)
project(BigHM)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp
        DataBase.h
        utils/utils.h
        utils/utils.cpp
        DataBase.cpp
        ExprChecker.h
        ExprChecker.cpp
        domain/DataBaseType.h
        domain/ColTable.h
        domain/DataBaseType.h
        domain/DataBaseType.cpp
)

add_executable(BigHM ${SOURCE_FILES})