//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdio>
#include <cstring>
#include <ctime>

#include <vector>
#include <stdexcept>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_result.hpp"

namespace mysqlpp
{

result::result(st_mysql_stmt* stmt) : stmt(0), current_row(0), metadata(0)
{
	iss.imbue(std::locale::classic());

	if (mysql_stmt_store_result(stmt) == 0)
	{
		throw exception(mysql_stmt_error(stmt));
	}

	metadata = mysql_stmt_result_metadata(stmt);
	if (!metadata)
	{
		throw exception("empty result");
	}

	field_count = mysql_stmt_field_count(stmt);
}

result::~result()
{
	if (metadata)
	{
		mysql_free_result(metadata);
	}
}

/*
bool result::next()
{
	++current_row;
	reset_field();

	if (data_index > 0)
	{
		if (myssql_stmt_bind_result(stmt, &binds[0]))
		{
			throw exception(mysql_stmt_error(stmt));
		}
	}
}
*/

void result::reset()
{
	binds.resize(0);
	binds.resize(field_count, st_mysql_bind());

	data.resize(0);
	data.resize(field_count, mysqlpp_data());

	for (int i = 0; i < field_count; ++i)
	{
		binds[i].buffer_type = MYSQL_TYPE_STRING;
		binds[i].buffer = data[i].buf;
		binds[i].buffer_length = sizeof(data[i].buf);
		binds[i].length = &data[i].length;
		binds[i].is_null = &data[i].is_null;
		binds[i].error = &data[i].error;
		
		data[i].ptr = data[i].buf;
	}
}

std::string result:: name(int index)
{
	if (index < 0 || index > field_count)
	{
		throw exception("invalid field index");
	}

	st_mysql_field* fields = mysql_fetch_fields(metadata);
	if (!fields)
	{
		throw exception("empty fields");
	}

	return fields[index].name;
}

int result::index(const std::string& name)
{
	st_mysql_field* fields = mysql_fetch_fields(metadata);
	if (!fields)
	{
		throw exception("empty fields");
	}

	for (int i = 0; i < field_count; ++i)
	{
		if (name == fields[i].name)
		{
			return i;
		}
	}

	return -1;
}

std::tm string_to_time(const std::string& time_str)
{
	if (strlen(time_str.c_str()) != time_str.size())
	{
		throw exception("bad value cast");
	}
	
	std::tm time = std::tm();

	double sec = 0;
	int n = sscanf_s(time_str.c_str(), "%d-%d-%d %d:%d:%lf",
		&time.tm_year,
		&time.tm_mon,
		&time.tm_mday,
		&time.tm_hour,
		&time.tm_min,
		&sec);

	if (n != 3 && n != 6)
	{
		throw exception("bad value cast");
	}

	time.tm_year -= 1900;
	time.tm_mon -= 1;
	time.tm_isdst -= 1;
	time.tm_sec = static_cast<int>(sec);

	if (mktime(&time) == -1)
	{
		throw exception("bad value cast");
	}

	return time;
}

bool result::fetch(int index, short int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, unsigned short int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, unsigned int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, long int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, unsigned long int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, long long int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, unsigned long long int &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, float &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, double &value)
{
	return fetch_data(index, value);
}

bool result::fetch(int index, long double &value)
{
	return fetch_data(index, value);
}
	
bool result::fetch(int index, std::string &value)
{
	if (index < 0 || index >= field_count)
	{
		throw exception("invalid field index");
	}

	mysqlpp_data& dat = data[index];
	if (dat.is_null)
	{
		return false;
	}

	value.assign(dat.ptr, dat.length);
	return true;
}

bool result::fetch(int index, std::ostream &value)
{
	if (index < 0 || index >= field_count)
	{
		throw exception("invalid field index");
	}

	mysqlpp_data& dat = data[index];
	if (dat.is_null)
	{
		return false;
	}

	value.write(dat.ptr, dat.length);
	return true;
}

bool result::fetch(const std::string& name, short int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, unsigned short int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, unsigned int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, long int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, unsigned long int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, long long int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, unsigned long long int &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, float &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, double &value)
{
	return fetch_data(index(name), value);
}

bool result::fetch(const std::string& name, long double &value)
{
	return fetch_data(index(name), value);
}
	
bool result::fetch(const std::string& name, std::string &value)
{
	return fetch(index(name), value);
}

bool result::fetch(const std::string& name, std::ostream &value)
{
	return fetch(index(name), value);
}

} // namespace mysqlpp
