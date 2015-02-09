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

		char error;
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

		void fetch_column(const st_mysql_column& column, unsigned char& value);
		void fetch_column(const st_mysql_column& column, short int& value);
		void fetch_column(const st_mysql_column& column, int& value);
		void fetch_column(const st_mysql_column& column, long long int& value);
		void fetch_column(const st_mysql_column& column, float& value);
		void fetch_column(const st_mysql_column& column, double& value);
		void fetch_column(const st_mysql_column& column, std::string& value);
		void fetch_column(const st_mysql_column& column, st_time& value);

		std::string time_to_str(const st_time& time);

	private:
		st_mysql_column& this_column(unsigned int index);
		st_mysql_column& this_column(const std::string& name);

		st_mysql_stmt* stmt;
		st_mysql_res* metadata;
		st_mysql_field* fields;

		unsigned int field_count;

		std::vector<st_mysql_bind> binds;
		std::vector<st_mysql_column> columns;
	};

} // namespace mysqlpp

#endif // MYSQLPP_RESULT_HPP
