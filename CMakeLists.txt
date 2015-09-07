cmake_minimum_required (VERSION 2.8)

project (mysqlpp)

find_library (MySQLClient_LIBRARIES NAMES mysqlclient)

add_executable (

	mysqlpp

	mysqlpp.hpp

	main.cpp
	mysqlpp_connection.cpp
	mysqlpp_result.cpp
	mysqlpp_statement.cpp

)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "-Wall -O2 -g")
endif()

target_link_libraries (mysqlpp ${MySQLClient_LIBRARIES})
