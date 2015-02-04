//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_RESULT_HPP
#define MYSQLPP_RESULT_HPP

#include <cstring>
#include <string>
#include <vector>
#include <sstream>

#include <mysql/mysql.h>

#include "mysqlpp_result.hpp"

namespace mysqlpp
{
	/*bind_data() :
	ptr(0),
		length(0),
		is_null(0),
		error(0)*/
struct mysqlpp_data
{
	mysqlpp_data() : is_null(0), error(0), length(0), ptr(0)
	{
		std::memset(&buf, 0, sizeof(buf));
	}

	my_bool is_null;
	my_bool error;

	unsigned long length;

	char* ptr;
	char buf[128];
	std::vector<char> vbuf;
};


class result
{
public:
	result(st_mysql_stmt* stmt);
	~result();

	void reset();
	std::string name(int index);
	int index(const std::string& name);

	template<typename T>
	T get(const std::string& name)
	{
		T value = T();
		if (!fetch(name, value))
		{
			throw exception("null value fetch");
		}

		return value;
	}

	template<typename T>
	T get(int index)
	{
		T value = T();
		if (!fetch(index, value))
		{
			throw exception("null value fetch");
		}
	}

private:
	bool fetch(int index, short int &value);
	bool fetch(int index, unsigned short int &value);
	bool fetch(int index, int &value);
	bool fetch(int index, unsigned int &value);
	bool fetch(int index, long int &value);
	bool fetch(int index, unsigned long int &value);
	bool fetch(int index, long long int &value);
	bool fetch(int index, unsigned long long int &value);
	bool fetch(int index, float &value);
	bool fetch(int index, double &value);
	bool fetch(int index, long double &value);
	
	bool fetch(int index, std::string &value);
	bool fetch(int index, std::ostream &value);

	template<typename T> bool do_fetch(T& value)
	{
		mysqlpp_data &d = data[field_index];
		if (d.is_null)
		{
			return false;
		}

		std::string str(d.ptr, d.length);
		iss.clear();
		iss.str(str);
		
		T value_;
		iss >> value_;
		if (iss.fail() || !std::ws(iss).eof())
		{
			throw exception("bad value cast");
		}

		if (std::numeric_limits<T>::is_integer &&
			!std::numeric_limits<T>::is_signed &&
			str.find('-') != std::string::npos &&
			&& value != 0)
		{
			throw exception("bad value cast");
		}

		value = static_cast<T>(value_);
		return true;
	}

	st_mysql_stmt* stmt;
	st_mysql_res* metadata;
	
	int field_index;
	unsigned int field_count;
	unsigned int current_row;
	
	std::vector<st_mysql_bind> binds;
	std::vector<mysqlpp_data> data;

	std::istringstream iss;
};

} // namespace mysqlpp

#endif // MYSQLPP_RESULT_HPP
