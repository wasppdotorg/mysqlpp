//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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

std::string result:: name(int index)
{
	if (index < 0 || index > field_count)
	{
		throw exception("invalid field_index");
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

	for (std::size_t i = 0; i < field_count; ++i)
	{
		if (name == fields[i].name)
		{
			return i;
		}
	}

	return -1;
}

void result::reset()
{
	binds.resize(0);
	binds.resize(field_count, st_mysql_bind());

	bind_data.resize(0);
	bind_data.resize(field_count, st_mysql_bind_data());

	for (std::size_t i = 0; i < field_count; ++i)
	{
		binds[i].buffer_type = MYSQL_TYPE_STRING;
		binds[i].buffer = bind_data[i].buf;
		binds[i].buffer_length = sizeof(bind_data[i].buf);
		binds[i].length = &bind_data[i].length;
		binds[i].is_null = &bind_data[i].is_null;
		binds[i].error = &bind_data[i].error;
		
		bind_data[i].ptr = bind_data[i].buf;
	}
}

} // namespace mysqlpp
