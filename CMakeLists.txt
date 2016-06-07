cmake_minimum_required (VERSION 3.0)

set (TARGET mysqlpp)
project (${TARGET})

add_definitions(-std=c++11)
aux_source_directory (. Source_DIRECTORIES)
add_executable (${TARGET} ${Source_DIRECTORIES})

include_directories (/usr/include /usr/local/include)

find_library (MySQLClient_LIBRARIES NAMES mysqlclient PATHS /usr/lib/mysql /usr/local/lib/mysql)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "-g -Wall -O2 -DNDEBUG")
endif()

target_link_libraries (${TARGET} ${MySQLClient_LIBRARIES})

