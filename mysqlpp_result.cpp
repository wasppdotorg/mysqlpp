//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <stdexcept>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_result.hpp"

namespace mysqlpp
{

result::result(st_mysql_stmt* stmt)
{
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

} // namespace mysqlpp
