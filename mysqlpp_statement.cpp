//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mysql/mysql.h>

#include "mysqlpp_statement.hpp"

namespace mysqlpp
{

statement::statement(MYSQL* mysql, const std::string& query)
{
	stmt = mysql_stmt_init(mysql);

	try
	{
		if (!stmt)
		{
			throw std::exception("statement_init failed");
		}

		if (mysql_stmt_prepare(stmt, query.c_str(), query.size()) != 0)
		{
			throw std::exception(mysql_stmt_error(stmt));
		}

		param_count = mysql_stmt_param_count(stmt);
	}
	catch (...)
	{
		mysql_stmt_close(stmt);
	}
}

statement::~statement()
{
	mysql_stmt_close(stmt);
}

unsigned long long statement::execute()
{
	if (mysql_stmt_execute(stmt) != 0)
	{
		throw std::exception(mysql_stmt_error(stmt));
	}

	return mysql_stmt_affected_rows(stmt);
}

result* statement::query()
{
	if (mysql_stmt_execute(stmt) != 0)
	{
		throw std::exception(mysql_stmt_error(stmt));
	}

	return new result(stmt);
}

} // namespace mysqlpp
