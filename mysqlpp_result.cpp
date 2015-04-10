/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "mysqlpp.hpp"

namespace mysqlpp
{

	result::result(st_mysql_stmt* stmt_) : stmt(stmt_), metadata(0), fields(0)
	{
		metadata = mysql_stmt_result_metadata(stmt);

		try
		{
			if (!metadata)
			{
				throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
			}

			bind();

			if (mysql_stmt_store_result(stmt) != 0)
			{
				throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
			}
		}
		catch (...)
		{
			mysql_free_result(metadata);
			throw;
		}
	}

	result::~result()
	{
		mysql_free_result(metadata);
	}

	bool result::bind()
	{
		field_count = mysql_stmt_field_count(stmt);
		if (field_count == 0)
		{
			return false;
		}

		fields = mysql_fetch_fields(metadata);

		columns.resize(0);
		columns.resize(field_count, st_mysql_column());

		binds.resize(0);
		binds.resize(field_count, st_mysql_bind());

		for (std::size_t i = 0; i < field_count; ++i)
		{
			columns[i].name = std::string(fields[i].name);
			columns[i].type = (fields[i].type == MYSQL_TYPE_DATETIME ? MYSQL_TYPE_STRING : fields[i].type);
			columns[i].length = fields[i].length;
			columns[i].buffer.resize(0);
			columns[i].buffer.resize(fields[i].length);

			binds[i].buffer_type = columns[i].type;
			binds[i].buffer_length = columns[i].length;
			binds[i].buffer = &columns[i].buffer.front();
			binds[i].length = &columns[i].length;
			binds[i].is_unsigned = columns[i].is_unsigned;
			binds[i].is_null = &columns[i].is_null;
			binds[i].error = &columns[i].error;
		}

		if (mysql_stmt_bind_result(stmt, &binds.front()) != 0)
		{
			throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
		}

		return true;
	}

	unsigned long long int result::num_rows()
	{
		return mysql_stmt_num_rows(stmt);
	}

	bool result::fetch(bool is_proc)
	{
		if (is_proc)
		{
			return fetch_proc_result();
		}

		return fetch_stmt_result();
	}

	void result::fetch_column(const st_mysql_column& column, unsigned char& value)
	{
		value = static_cast<unsigned char>(column.buffer.front());
	}

	void result::fetch_column(const st_mysql_column& column, short int& value)
	{
		value = static_cast<short int>(column.buffer.front());
	}

	void result::fetch_column(const st_mysql_column& column, unsigned short int& value)
	{
		value = static_cast<unsigned short int>(column.buffer.front());
	}

	void result::fetch_column(const st_mysql_column& column, int& value)
	{
		value = static_cast<int>(column.buffer.front());
	}

	void result::fetch_column(const st_mysql_column& column, unsigned int& value)
	{
		value = static_cast<unsigned int>(column.buffer.front());
	}

	void result::fetch_column(const st_mysql_column& column, long long int& value)
	{
		value = *reinterpret_cast<long long int*>(const_cast<char*>(&column.buffer.front()));
	}

	void result::fetch_column(const st_mysql_column& column, unsigned long long int& value)
	{
		value = *reinterpret_cast<unsigned long long int*>(const_cast<char*>(&column.buffer.front()));
	}

	void result::fetch_column(const st_mysql_column& column, float& value)
	{
		value = *reinterpret_cast<float*>(const_cast<char*>(&column.buffer.front()));
	}

	void result::fetch_column(const st_mysql_column& column, double& value)
	{
		value = *reinterpret_cast<double*>(const_cast<char*>(&column.buffer.front()));
	}

	void result::fetch_column(const st_mysql_column& column, std::string& value)
	{
		value.assign(&column.buffer.front(), column.length);
	}

	bool result::is_null(unsigned int index)
	{
		if (index >= field_count)
		{
			throw exception(__FILE__, __LINE__, "invalid column_index");
		}

		return columns.at(index).is_null == 1;
	}

	bool result::is_null(const std::string& name)
	{
		unsigned int i = 0;
		for (; i < field_count; ++i)
		{
			if (name == columns[i].name)
			{
				break;
			}
		}

		if (i == field_count)
		{
			throw exception(__FILE__, __LINE__, "invalid column_name");
		}

		return columns.at(i).is_null == 1;
	}

	bool result::fetch_stmt_result()
	{
		if (mysql_stmt_fetch(stmt) == MYSQL_NO_DATA)
		{
			return false;
		}

		return true;
	}

	bool result::fetch_proc_result()
	{
		mysql_free_result(metadata);
		metadata = mysql_stmt_result_metadata(stmt);

		try
		{
			if (!metadata)
			{
				throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
			}

			if (!bind())
			{
				return false;
			}

			while (1)
			{
				int fetch_result = mysql_stmt_fetch(stmt);

				if (fetch_result == 1 || fetch_result == MYSQL_NO_DATA)
				{
					break;
				}
			}

			if (mysql_stmt_next_result(stmt) > 0)
			{
				throw exception(__FILE__, __LINE__, mysql_stmt_error(stmt));
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	st_mysql_column& result::get_column(unsigned int index)
	{
		if (index >= field_count)
		{
			throw exception(__FILE__, __LINE__, "invalid column_index");
		}

		return columns.at(index);
	}

	st_mysql_column& result::get_column(const std::string& name)
	{
		unsigned int i = 0;
		for (; i < field_count; ++i)
		{
			if (name == columns[i].name)
			{
				break;
			}
		}

		if (i == field_count)
		{
			throw exception(__FILE__, __LINE__, "invalid column_name");
		}

		return columns.at(i);
	}

} // namespace mysqlpp
