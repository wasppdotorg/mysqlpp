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

#include "mysqlpp_exception.hpp"
#include "mysqlpp_result.hpp"

namespace mysqlpp
{

struct mysqlpp_param
{
	mysqlpp_param() : is_null_(0), length_(0), buffer_(0)
	{
	    field_type_ = MYSQL_TYPE_NULL;
	}

	void set(enum_field_types field_type, const char* begin, const char* end)
	{
		field_type_ = field_type;
		length_ = end - begin;
		buffer_ = const_cast<char*>(begin);
	}

	void set(enum_field_types field_type, const std::tm& time)
	{
		char buf[64] = { 0 };
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &time);

		set(field_type, std::string(buf));
	}

	void set(enum_field_types field_type, const std::string& str)
	{
		field_type_ = field_type;
		value_ = str;
		length_ = value_.size();
		buffer_ =  const_cast<char*>(value_.c_str());
	}

	void make_bind(st_mysql_bind* bind)
	{
		if (field_type_ == MYSQL_TYPE_NULL)
		{
			is_null_ = 1;
			bind->is_null = &is_null_;

			bind->buffer_type = field_type_;
			return;
		}

		is_null_ = 0;
		bind->is_null = &is_null_;

		bind->buffer_type = field_type_;
		bind->buffer = buffer_;
		bind->buffer_length = length_;
		bind->length = &length_;
	}

	my_bool is_null_;
	enum_field_types field_type_;

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
	void param(long long int value);
	void param(float value);
	void param(double value);

	void param(const std::tm& value);
	void param(const std::string& value);
	void param(std::istream& value);

	void param_null();
	
	unsigned long long execute();
	result* execute_query();

private:
	mysqlpp_param& param_at(int param_index)
	{
		if (param_index < 0 || param_index >= param_count)
		{
			throw exception("invalid param_index");
		}

		return params[param_index];
	}

    template<typename T> void set_param(enum_field_types field_type, T value)
	{
		oss.str(std::string());
		if (!std::numeric_limits<T>::is_integer)
		{
			oss << std::setprecision(std::numeric_limits<T>::digits10 + 1);
		}
		oss << value;

		param_at(param_index).set(field_type, oss.str());
		++param_index;
	}

	st_mysql_stmt* stmt;

	int param_count;
	int param_index;

	std::vector<mysqlpp_param> params;
	std::vector<st_mysql_bind> binds;

	std::ostringstream oss;
};

} // namespace mysqlpp

#endif // MYSQLPP_STATEMENT_HPP
