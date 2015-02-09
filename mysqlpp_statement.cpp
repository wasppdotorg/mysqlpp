//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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
			throw;
		}
	}

	statement::~statement()
	{
		mysql_stmt_close(stmt);
	}

	void statement::param(const unsigned char& value)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_TINY;
		bind.buffer = (char*)&value;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const short int& value)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_SHORT;
		bind.buffer = (char*)&value;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const int& value)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_LONG;
		bind.buffer = (char*)&value;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const long long int& value)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_LONGLONG;
		bind.buffer = (char*)&value;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const float& value, unsigned long length)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_FLOAT;
		bind.buffer = const_cast<float*>(&value);
		bind.is_null = 0;

		length = sizeof(value);
		bind.length = &length;
	}

	void statement::param(const double& value, unsigned long length)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_DOUBLE;
		bind.buffer = const_cast<double*>(&value);
		bind.is_null = 0;

		length = sizeof(value);
		bind.length = &length;
	}

	void statement::param(const std::string& value, unsigned long& length)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_VAR_STRING;
		bind.buffer = const_cast<char*>(value.c_str());
		bind.buffer_length = length;
		bind.is_null = 0;
		bind.length = &length;
	}

	void statement::param(const st_mysql_time& value)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_DATE;
		bind.buffer = (char*)&value;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param_null(my_bool is_null)
	{
		st_mysql_bind& bind = this_bind();

		bind.buffer_type = MYSQL_TYPE_NULL;
		bind.is_null = &is_null;
	}

	unsigned long long statement::execute()
	{
		unsigned long long affected_rows = -1;

		try
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

			affected_rows = mysql_stmt_affected_rows(stmt);
		}
		catch (...)
		{
			throw;
		}

		return affected_rows;
	}

	result* statement::query()
	{
		try
		{
			if (mysql_stmt_execute(stmt) != 0)
			{
				throw exception(mysql_stmt_error(stmt));
			}
		}
		catch (...)
		{
			throw;
		}

		return new result(stmt);
	}

} // namespace mysqlpp
