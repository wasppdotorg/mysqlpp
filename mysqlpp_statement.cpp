/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

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

	void statement::param(const uint8_t& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_TINY;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const int16_t& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_SHORT;
		b.buffer = (void*)&value;
		b.is_unsigned = 0;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const uint16_t& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_SHORT;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const int32_t& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 0;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const uint32_t& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const int64_t& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONGLONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 0;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const uint64_t& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_LONGLONG;
		b.buffer = (void*)&value;
		b.is_unsigned = 1;
		b.is_null = 0;
		b.length = 0;
	}

	void statement::param(const float& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_FLOAT;
		b.buffer = (void*)&value;
		b.is_null = 0;

		lengths[bind_index - 1] = sizeof(value);
		b.length = &lengths[bind_index - 1];
	}

	void statement::param(const double& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_DOUBLE;
		b.buffer = (void*)&value;
		b.is_null = 0;

		lengths[bind_index - 1] = sizeof(value);
		b.length = &lengths[bind_index - 1];
	}

	void statement::param(const std::string& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_STRING;
		b.buffer = (void*)value.c_str();
		b.is_null = 0;

		lengths[bind_index - 1] = value.size();
		b.buffer_length = lengths[bind_index - 1];
		b.length = &lengths[bind_index - 1];
	}

	void statement::param_blob(const std::string& value)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_BLOB;
		b.buffer = (void*)value.c_str();
		b.is_null = 0;

		lengths[bind_index - 1] = value.size();
		b.buffer_length = lengths[bind_index - 1];
		b.length = &lengths[bind_index - 1];
	}

	void statement::param_null(char is_null)
	{
		st_mysql_bind& b = get_bind();

		b.buffer_type = MYSQL_TYPE_NULL;
		b.is_null = &is_null;
	}

	uint64_t statement::execute()
	{
		uint64_t affected_rows = 0;

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
