add_library(MyDataBase
        conditionParser/ExprChecker.cpp conditionParser/ExprChecker.h
        dataBase/DataBase.cpp dataBase/DataBase.h
        domain/ColTable.cpp domain/ColTable.h
        domain/DataBaseType.cpp domain/DataBaseType.h
        domain/Element.cpp domain/Element.h
        table/Table.cpp table/Table.h
        utils/utils.cpp utils/utils.h
)
target_include_directories(MyDataBase INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})