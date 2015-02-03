//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_STATEMENT_HPP
#define MYSQLPP_STATEMENT_HPP

#include <vector>
#include <string>

#include <mysql/mysql.h>

#include "mysqlpp_result.hpp"

namespace mysqlpp
{

class statement
{
public:
	statement(MYSQL* mysql, const std::string& query_);
	~statement();

	void bind();
	unsigned long long execute();
	result* query();

private:
	MYSQL_STMT* stmt;
	
	int param_count;

	std::vector<MYSQL_BIND> binds;
	int bind_index;
};

} // namespace mysqlpp

#endif // MYSQLPP_STATEMENT_HPP
