//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_HPP
#define MYSQLPP_HPP

#include <ctime>

#include <stdexcept>
#include <vector>
#include <string>

namespace mysqlpp
{

	typedef st_mysql_time datetime;

	/*
	struct datetime : st_mysql_time
	{
		datetime()
		{
			std::time_t raw_time = std::time(0);	
			std::tm time = *std::localtime(&raw_time);

			year = static_cast<unsigned int>(time.tm_year) + 1900;
			month = static_cast<unsigned int>(time.tm_mon) + 1;
			day = static_cast<unsigned int>(time.tm_mday);

			hour = static_cast<unsigned int>(time.tm_hour);
			minute = static_cast<unsigned int>(time.tm_min);
			second = static_cast<unsigned int>(time.tm_sec);
		}
	};
	*/

	struct st_mysql_column
	{
		st_mysql_column() : buffer(0), length(0), is_null(0), error(0)
		{
		}

		enum_field_types type;
		std::string name;

		std::vector<char> buffer;
		unsigned long length;

		char is_null;
		char error;
	};

	class exception : public std::runtime_error
	{
	public:
		exception(const std::string& what) : std::runtime_error(what)
		{

		}
	};

	class result
	{
	public:
		result(st_mysql_stmt* stmt_);
		~result();

		unsigned long long num_rows();
		bool fetch();

		template<typename T>
		T get(unsigned int index)
		{
			st_mysql_column& column = this_column(index);
			if (column.is_null)
			{
				throw exception("null value field");
			}

			T value = T();
			fetch_column(column, value);

			return value;
		}

		template<typename T>
		T get(const std::string& name)
		{
			st_mysql_column& column = this_column(name);
			if (column.is_null)
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
		void fetch_column(const st_mysql_column& column, datetime& value);

		std::string datetime_str(const datetime& time);

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

	class statement
	{
	public:
		statement(st_mysql* mysql, const std::string& query);
		~statement();

		void param(const unsigned char& value);
		void param(const short int& value);
		void param(const int& value);
		void param(const long long int& value);
		void param(const float& value, unsigned long length = 0);
		void param(const double& value, unsigned long length = 0);
		void param(const std::string& value, unsigned long& length);
		void param(const datetime& value);
		void param_null(char is_null = 1);

		unsigned long long execute();
		result* query();

	private:
		st_mysql_bind& this_bind();
		
		st_mysql_stmt* stmt;

		int param_count;
		int bind_index;

		std::vector<st_mysql_bind> binds;
	};

	class connection
	{
	public:
		connection(const std::string& host, const std::string& userid, const std::string& passwd, const std::string& dbname, unsigned int port = 3306, bool pooled_ = false);
		~connection();

		statement* prepare(const std::string& query);

	private:
		st_mysql* mysql;

		std::tm released;
		bool pooled;

	};

} // namespace mysqlpp

#endif // MYSQLPP_HPP
