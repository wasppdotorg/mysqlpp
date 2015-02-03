//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_STATEMENT_HPP
#define MYSQLPP_STATEMENT_HPP

#include <iomanip>
#include <vector>
#include <string>
#include <istream>
#include <sstream>
#include <limits>

#include <mysql/mysql.h>

#include "mysqlpp_result.hpp"

namespace mysqlpp
{

struct st_mysql_param
{
	st_mysql_param() : is_null_(1), is_blob_(false), length_(0), buffer_(0)
	{
	}

	/*
	void set(const char* begin, const char* end, bool is_blob = false)
	{
		is_null_ = 0;
		is_blob_ = is_blob;

		length_ = end - begin;
		buffer_ = const_cast<char*>(begin);
	}
	*/

	void set(const std::string& str)
	{
		is_null_ = 0;

		value_ = str;
		length_ = value_.size();
		buffer_ =  const_cast<char*>(value_.c_str());
	}

	void make_bind(st_mysql_bind* bind)
	{
		bind->is_null = &is_null_;
		if (is_null_)
		{
			bind->buffer_type = MYSQL_TYPE_NULL;
			return;
		}

		bind->buffer_type = MYSQL_TYPE_STRING;
		if (is_blob_)
		{
			bind->buffer_type = MYSQL_TYPE_BLOB;
		}

		bind->buffer = buffer_;
		bind->buffer_length = length_;
		bind->length = &length_;
	}

	my_bool is_null_;
	bool is_blob_;

	std::string value_;
	unsigned long length_;

	void *buffer_;
};

class statement
{
public:
	statement(st_mysql* mysql, const std::string& query);
	~statement();

	void param(signed char value);

	void param(short int value);
	void param(int value);

	void param(float value);

	void param(long int value);
	void param(long long int value);

	void param(double value);
	void param(long double value);

	void param(unsigned int value);
	void param(unsigned long int value);
	void param(unsigned long long int value);

	unsigned long long execute();
	result* execute_query();

private:
	template<typename T> void set_param(T value)
	{
		oss.str(std::string());
		if (!std::numeric_limits<T>::is_integer)
		{
			oss << std::setprecision(std::numeric_limits<T>::digits10 + 1);
		}
		oss << value;

		params[param_index].set(oss.str());
		++param_index;
	}

	st_mysql_stmt* stmt;

	int param_count;
	int param_index;

	std::vector<st_mysql_param> params;
	std::vector<st_mysql_bind> binds;

	std::ostringstream oss;
};

} // namespace mysqlpp

#endif // MYSQLPP_STATEMENT_HPP
