//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_RESULT_HPP
#define MYSQLPP_RESULT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_result.hpp"

namespace mysqlpp
{

struct st_mysql_column
{
	st_mysql_column() : buffer(0), length(0), error(0)
	{
	}

	enum_field_types type;
	std::string name;

	std::vector<char> buffer;
	unsigned long length;

	my_bool error;
};

class result
{
public:
	result(st_mysql_stmt* stmt_);
	~result();

	unsigned long long num_rows();
	bool fetch();

	template<typename T>
	T field(unsigned int index)
	{
		st_mysql_column& column = this_column(index);
		if (column.type == MYSQL_TYPE_NULL)
		{
			throw exception("null value field");
		}

		T value = T();
		fetch_column(column, value);

		return value;
	}

	template<typename T>
	T field(const std::string& name)
	{
		st_mysql_column& column = this_column(name);
		if (column.type == MYSQL_TYPE_NULL)
		{
			throw exception("null value field");
		}

		T value = T();
		fetch_column(column, value);

		return value;
	}

	void fetch_column(const st_mysql_column& column, unsigned char& value)
	{
		value = static_cast<unsigned char>(column.buffer.front());
	}

	void fetch_column(const st_mysql_column& column, short int& value)
	{
		value = static_cast<short int>(column.buffer.front());
	}

	void fetch_column(const st_mysql_column& column, int& value)
	{
		value = static_cast<int>(column.buffer.front());
	}

	void fetch_column(const st_mysql_column& column, long long int& value)
	{
		value = static_cast<long long int>(column.buffer.front());
	}

	void fetch_column(const st_mysql_column& column, float& value)
	{
		iss.clear();
		float value_ = 0;

		std::string a(std::string(column.buffer.begin(), column.buffer.end()));
		std::cout << a << std::endl;

		iss.str(std::string(column.buffer.begin(), column.buffer.end()));
		iss >> value_;

		std::cout << value_ << std::endl;

		if (iss.fail() || !std::ws(iss).eof())
		{
			throw exception("cast failed");
		}

		value = value_;
	}

	void fetch_column(const st_mysql_column& column, double& value)
	{
		iss.clear();
		double value_ = 0;

		iss.str(std::string(column.buffer.begin(), column.buffer.end()));
		iss >> value_;

		if (iss.fail() || !std::ws(iss).eof())
		{
			throw exception("casst failed");
		}

		value = value_;
	}

	void fetch_column(const st_mysql_column& column, std::string& value)
	{
		value = std::string(column.buffer.begin(), column.buffer.end());
	}

	void fetch_column(const st_mysql_column& column, st_mysql_time& value)
	{
		value = *reinterpret_cast<st_mysql_time*>(column.buffer.front());
	}

private:
	st_mysql_column& this_column(unsigned int index)
	{
		if (index >= field_count)
		{
			throw exception("invalid column_index");
		}

		return columns.at(index);
	}

	st_mysql_column& this_column(const std::string& name)
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
			throw exception("invalid column_name");
		}

		return columns.at(i);
	}

	st_mysql_stmt* stmt;
	st_mysql_res* metadata;
	st_mysql_field* fields;

	unsigned int field_count;

	std::vector<st_mysql_bind> binds;
	std::vector<st_mysql_column> columns;

	std::istringstream iss;
};

} // namespace mysqlpp

#endif // MYSQLPP_RESULT_HPP
