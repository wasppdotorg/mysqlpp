//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <sstream>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_statement.hpp"

namespace mysqlpp
{

statement::statement(st_mysql* mysql, const std::string& query)
{
	stmt = mysql_stmt_init(mysql);
	
	try
	{
		if (!stmt)
		{
			throw exception("stmt_init failed");
		}

		if (mysql_stmt_prepare(stmt, query.c_str(), query.size()) != 0)
		{
			throw exception(mysql_stmt_error(stmt));
		}

		param_count = mysql_stmt_param_count(stmt);

		binds.resize(0);
		binds.resize(param_count, st_mysql_bind());

		bind_index = 0;
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

void statement::param(unsigned char* value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_TINY;
	bind.buffer = (char*)value;
	bind.is_null = 0;
	bind.length = &length;
}

void statement::param(short int* value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_SHORT;
	bind.buffer = (char*)value;
	bind.is_null = 0;
	bind.length = &length;
}

void statement::param(int* value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_LONG;
	bind.buffer = (char*)value;
	bind.is_null = 0;
	bind.length = &length;
}

void statement::param(long long int* value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_LONGLONG;
	bind.buffer = (char*)value;
	bind.is_null = 0;
	bind.length = &length;
}

void statement::param(float* value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_FLOAT;
	bind.buffer = value;
	bind.is_null = 0;

	length = sizeof(value);
	bind.length = &length;
}

void statement::param(double* value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_DOUBLE;
	bind.buffer = value;
	bind.is_null = 0;

	length = sizeof(value);
	bind.length = &length;
}

void statement::param(const std::tm& value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	char buf[64] = { 0 };
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &value);

	std::string value_(buf);
	bind.buffer_type = MYSQL_TYPE_DATETIME;
	bind.buffer = const_cast<char*>(value_.c_str());
	bind.is_null = 0;

	length = value_.size();
	bind.length = &length;
}

void statement::param(const std::string& value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_STRING;
	bind.buffer = const_cast<char*>(value.c_str());
	bind.is_null = 0;

	length = value.size();
	bind.length = &length;
}

void statement::param(std::istream& value, unsigned long length)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_BLOB;

	std::ostringstream ss;
	ss << value.rdbuf();
	bind.buffer = const_cast<char*>(ss.str().c_str());

	bind.is_null = 0;

	length = ss.str().size();
	bind.length = &length;
}

void statement::param_null(my_bool is_null_)
{
	st_mysql_bind& bind = this_bind();

	bind.buffer_type = MYSQL_TYPE_NULL;
	bind.is_null = &is_null_;
}

unsigned long long statement::execute()
{
	if (param_count > 0 && !binds.empty())
	{
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
