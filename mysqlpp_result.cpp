//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>
#include <vector>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_result.hpp"

namespace mysqlpp
{

result::result(st_mysql_stmt* stmt_) : stmt(stmt_), metadata(0), fields(0)
{
	metadata = mysql_stmt_result_metadata(stmt);

	try
	{
		if (!metadata)
		{
			throw exception(mysql_stmt_error(stmt));
		}

		field_count = mysql_stmt_field_count(stmt);
		if (field_count == 0)
		{
			throw exception("zero field_count");
		}

		fields = mysql_fetch_fields(metadata);

		if (mysql_stmt_store_result(stmt) != 0)
		{
			throw exception(mysql_stmt_error(stmt));
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

unsigned long long result::num_rows()
{
	return mysql_stmt_num_rows(stmt);
}

bool result::fetch()
{
	binds.resize(0);
	binds.resize(field_count, st_mysql_bind());

	columns.resize(0);
	columns.resize(field_count, st_mysql_column());

	for (std::size_t i = 0; i < field_count; ++i)
	{
		columns[i].name = std::string(fields[i].name);
		columns[i].buffer.resize(0);
		columns[i].buffer.resize(fields[i].length);

		binds[i].buffer_type = fields[i].type;
		binds[i].buffer = (char*)&columns[i].buffer.front();
		binds[i].is_null = &columns[i].is_null;
		binds[i].length = &columns[i].length;
		binds[i].error = &columns[i].error;
	}

	if (mysql_stmt_bind_result(stmt, &binds.front()) != 0)
	{
		throw exception(mysql_stmt_error(stmt));
	}

	int r = mysql_stmt_fetch(stmt);
	if (r == MYSQL_NO_DATA || r != MYSQL_DATA_TRUNCATED)
	{
		return false;
	}

	return true;
}

int result::field(unsigned int index)
{
	st_mysql_column& column = this_column(index);

	char* buffer = new char[column.length];
	*buffer = column.buffer.front();

	return static_cast<int>(*buffer);
}

int result::field(const std::string& name)
{
	st_mysql_column& column = this_column(name);
	char* buffer = new char[column.length];
	*buffer = column.buffer.front();

	return static_cast<int>(*buffer);
}

/*
std::string result::name(int index)
{
	if (index < 0 || index > field_count)
	{
		throw exception("invalid field index");
	}

	st_mysql_field* field = mysql_fetch_fields(metadata);
	if (!field)
	{
		throw exception("empty field");
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

bool result::fetch(int index, std::tm &value)
{
	mysqlpp_data& dat = data_at(index);
	if (dat.is_null)
	{
		return false;
	}

	std::string time_str;
	time_str.assign(dat.ptr, dat.length);

	if (strlen(time_str.c_str()) != time_str.size())
	{
		throw exception("bad value cast");
	}

	std::tm time = std::tm();

	double sec = 0;
	int n = sscanf(time_str.c_str(), "%d-%d-%d %d:%d:%lf",
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

	value = time;
	return true;
}

bool result::fetch(int index, std::string &value)
{
	mysqlpp_data& dat = data_at(index);
	if (dat.is_null)
	{
		return false;
	}

	value.assign(dat.ptr, dat.length);
	return true;
}

bool result::fetch(int index, std::ostream &value)
{
	mysqlpp_data& dat = data_at(index);
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
*/

} // namespace mysqlpp
