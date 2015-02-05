//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_statement.hpp"

#include <vld.h>
namespace mysqlpp
{

statement::statement(st_mysql* mysql, const std::string& query)
{
	stmt = mysql_stmt_init(mysql);
	oss.imbue(std::locale::classic());

	try
	{
		if (!stmt)
		{
			throw exception("statement_init failed");
		}

		if (mysql_stmt_prepare(stmt, query.c_str(), query.size()) != 0)
		{
			throw exception(mysql_stmt_error(stmt));
		}

		param_index = 0;
		param_count = mysql_stmt_param_count(stmt);

		params.resize(0);
		params.resize(param_count);

		binds.resize(0);
		binds.resize(param_count, st_mysql_bind());
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

void statement::param(signed char value)
{
	set_param(MYSQL_TYPE_TINY, value);
}

void statement::param(short int value)
{
	set_param(MYSQL_TYPE_SHORT, value);
}

void statement::param(int value)
{
	set_param(MYSQL_TYPE_LONG, value);
}

void statement::param(long long int value)
{
	set_param(MYSQL_TYPE_LONGLONG, value);
}

void statement::param(float value)
{
	set_param(MYSQL_TYPE_FLOAT, value);
}

void statement::param(double value)
{
	set_param(MYSQL_TYPE_DOUBLE, value);
}

void statement::param(const std::tm& value)
{
	param_at(param_index).set(MYSQL_TYPE_DATETIME, value);
	++param_index;
}

void statement::param(const std::string& value)
{
	param_at(param_index).set(MYSQL_TYPE_STRING, value.c_str(), value.c_str() + value.size());
	++param_index;
}

void statement::param(std::istream& value)
{
	std::ostringstream ss;
	ss << value.rdbuf();

	param_at(param_index).set(MYSQL_TYPE_BLOB, ss.str());
	++param_index;
}

void statement::param_null()
{
	param_at(param_index) = mysqlpp_param();
	++param_index;
}

unsigned long long statement::execute()
{
	if (!params.empty())
	{
		for (std::size_t i = 0; i < params.size(); ++i)
		{
			params[i].make_bind(&binds[i]);
		}

		if (mysql_stmt_bind_param(stmt, &binds.front()) != 0)
		{
			throw exception(mysql_stmt_error(stmt));
		}
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		throw exception(mysql_stmt_error(stmt));
	}

	return mysql_stmt_affected_rows(stmt);
}

result* statement::execute_query()
{
	if (mysql_stmt_execute(stmt) != 0)
	{
		throw exception(mysql_stmt_error(stmt));
	}

	return new result(stmt);
}

} // namespace mysqlpp
