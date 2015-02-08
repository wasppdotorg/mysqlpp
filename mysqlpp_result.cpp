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
		columns[i].type = fields[i].type;
		columns[i].buffer.resize(0);
		columns[i].buffer.resize(fields[i].length);

		binds[i].buffer_type = fields[i].type;
		binds[i].buffer = const_cast<char*>(&columns[i].buffer.front());
		binds[i].length = &columns[i].length;
		binds[i].error = &columns[i].error;
	}

	if (mysql_stmt_bind_result(stmt, &binds.front()) != 0)
	{
		throw exception(mysql_stmt_error(stmt));
	}

	if (mysql_stmt_fetch(stmt) != 0)
	{
		return false;
	}

	return true;
}

} // namespace mysqlpp
