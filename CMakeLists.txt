
cmake_minimum_required (VERSION 3.0)

set (TARGET mysqlpp)
project (${TARGET})

aux_source_directory (. Source_DIRECTORIES)
add_executable (${TARGET} ${Source_DIRECTORIES})

include_directories (/usr/include /usr/local/include)

find_library (MySQLClient_LIBRARIES NAMES mysqlclient PATHS /usr/lib/mysql /usr/local/lib/mysql)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
	set (CMAKE_COMPILER_IS_CLANGCXX 1)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
	set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fexceptions -O2 -DNDEBUG")
endif()

target_link_libraries (${TARGET} ${MySQLClient_LIBRARIES})

