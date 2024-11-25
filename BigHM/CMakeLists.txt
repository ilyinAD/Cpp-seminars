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


add_subdirectory(googletest)
enable_testing()
add_executable(my_tests test/conditionParser_test.cpp)
target_link_libraries(my_tests gtest gtest_main)

target_include_directories(my_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_sources(my_tests PRIVATE
        conditionParser/ExprChecker.cpp
        utils/utils.cpp
)
add_executable(BigHM ${SOURCE_FILES})