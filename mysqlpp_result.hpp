//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_RESULT_HPP
#define MYSQLPP_RESULT_HPP

#include <mysql/mysql.h>

#include "mysqlpp_result.hpp"

namespace mysqlpp
{

class result
{
public:
	result(st_mysql_stmt* stmt);
	~result();

private:
	unsigned int field_count;
	unsigned int current_row;

	st_mysql_res* metadata;
};

} // namespace mysqlpp

#endif // MYSQLPP_RESULT_HPP
