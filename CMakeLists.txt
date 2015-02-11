cmake_minimum_required (VERSION 2.6)

project (mysqlpp)

find_library(MySQLClient_LIBRARIES NAMES mysqlclient)

add_executable(mysqlpp mysqlpp.hpp main.cpp mysqlpp_connection.cpp mysqlpp_result.cpp mysqlpp_statement.cpp)

target_link_libraries(mysqlpp ${MySQLClient_LIBRARIES})
