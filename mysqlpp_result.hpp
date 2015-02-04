//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_RESULT_HPP
#define MYSQLPP_RESULT_HPP

#include <string>
#include <vector>
#include <sstream>

#include <mysql/mysql.h>

#include "mysqlpp_result.hpp"

namespace mysqlpp
{

struct st_mysql_bind_data
{
	st_mysql_bind_data()
	{

	}

	char buf[128];
	std::vector<char> vbuf;
	char* ptr;

	unsigned long length;

	my_bool is_null;
	my_bool error;
};


class result
{
public:
	result(st_mysql_stmt* stmt);
	~result();

	void reset();
	std::string name(int field_index);
	int index(const std::string& name);

private:
	/*template<typename T>
		bool do_fetch(int col,T &v)
		{
		bind_data &d=at(col);
		if(d.is_null)
		return false;

		v=parse_number<T>(std::string(d.ptr,d.length),fmt_);
		return true;
		}*/

	template<typename T> bool fetch(T& value)
	{
		st_mysql_bind_data &data = bind_data[field_index];
		if (data.is_null)
		{
			return false;
		}



	}

	st_mysql_stmt* stmt;
	st_mysql_res* metadata;
	
	int field_index;
	unsigned int field_count;
	unsigned int current_row;
	
	std::vector<st_mysql_bind> binds;
	std::vector<st_mysql_bind_data> bind_data;

	std::istringstream iss;
};

} // namespace mysqlpp

#endif // MYSQLPP_RESULT_HPP
