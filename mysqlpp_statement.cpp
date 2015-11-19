/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "mysqlpp.hpp"

namespace mysqlpp
{

	statement::statement(st_mysql* mysql, const std::string& query)
	{
		try
		{
			stmt = mysql_stmt_init(mysql);
			if (!stmt)
			{
				throw exception(__FILE__, __LINE__, "stmt_init failed");
			}

			if (mysql_stmt_prepare(stmt, query.c_str(), query.size()) != 0)
			{
				throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
			}

			param_count = mysql_stmt_param_count(stmt);

			binds.resize(0);
			binds.resize(param_count, st_mysql_bind());

			lengths.resize(0);
			lengths.resize(param_count, 0);

			bind_index = 0;
		}
		catch (...)
		{
			if (stmt)
			{
				mysql_stmt_close(stmt);
			}

			throw;
		}
	}

	statement::~statement()
	{
		mysql_stmt_close(stmt);
	}

	void statement::param(const unsigned char& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_TINY;
		bind.buffer = (void*)&value;
		bind.is_unsigned = 1;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const short int& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_SHORT;
		bind.buffer = (void*)&value;
		bind.is_unsigned = 0;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const unsigned short int& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_SHORT;
		bind.buffer = (void*)&value;
		bind.is_unsigned = 1;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const int& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_LONG;
		bind.buffer = (void*)&value;
		bind.is_unsigned = 0;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const unsigned int& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_LONG;
		bind.buffer = (void*)&value;
		bind.is_unsigned = 1;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const long long int& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_LONGLONG;
		bind.buffer = (void*)&value;
		bind.is_unsigned = 0;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const unsigned long long int& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_LONGLONG;
		bind.buffer = (void*)&value;
		bind.is_unsigned = 1;
		bind.is_null = 0;
		bind.length = 0;
	}

	void statement::param(const float& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_FLOAT;
		bind.buffer = (void*)&value;
		bind.is_null = 0;

		lengths[bind_index - 1] = sizeof(value);
		bind.length = &lengths[bind_index - 1];
	}

	void statement::param(const double& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_DOUBLE;
		bind.buffer = (void*)&value;
		bind.is_null = 0;

		lengths[bind_index - 1] = sizeof(value);
		bind.length = &lengths[bind_index - 1];
	}

	void statement::param(const std::string& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_STRING;
		bind.buffer = (void*)value.c_str();
		bind.is_null = 0;

		lengths[bind_index - 1] = value.size();
		bind.buffer_length = lengths[bind_index - 1];
		bind.length = &lengths[bind_index - 1];
	}

	void statement::param_blob(const std::string& value)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_BLOB;
		bind.buffer = (void*)value.c_str();
		bind.is_null = 0;

		lengths[bind_index - 1] = value.size();
		bind.buffer_length = lengths[bind_index - 1];
		bind.length = &lengths[bind_index - 1];
	}

	void statement::param_null(char is_null)
	{
		st_mysql_bind& bind = get_bind();

		bind.buffer_type = MYSQL_TYPE_NULL;
		bind.is_null = &is_null;
	}

	unsigned long long int statement::execute()
	{
		unsigned long long int affected_rows = 0;

		try
		{
			if (param_count > 0 && !binds.empty())
			{
				if (mysql_stmt_bind_param(stmt, &binds.front()) != 0)
				{
					throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
				}
			}

			if (mysql_stmt_execute(stmt) != 0)
			{
				throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
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
			if (param_count > 0 && !binds.empty())
			{
				if (mysql_stmt_bind_param(stmt, &binds.front()) != 0)
				{
					throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
				}
			}

			if (mysql_stmt_execute(stmt) != 0)
			{
				throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
			}
		}
		catch (...)
		{
			throw;
		}

		return new result(stmt);
	}

	st_mysql_bind& statement::get_bind()
	{
		if (bind_index < 0 || bind_index >= param_count)
		{
			throw exception(__FILE__, __LINE__, "invalid bind_index");
		}

		return binds[bind_index++];
	}

} // namespace mysqlpp
