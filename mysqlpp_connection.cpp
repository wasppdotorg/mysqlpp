//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_statement.hpp"
#include "mysqlpp_connection.hpp"

namespace mysqlpp
{

	connection::connection(const std::string& host, const std::string& userid, const std::string& passwd, const std::string& dbname, unsigned int port)
{
	mysql = mysql_init(0);

	try
	{
		if (!mysql)
		{
			throw exception("mysql_init failed");
		}

		if (!mysql_real_connect(mysql, host.c_str(), userid.c_str(), passwd.c_str(), dbname.c_str(), port, 0, 0))
		{
			throw exception(mysql_error(mysql));
		}
	}
	catch (...)
	{
		mysql_close(mysql);
		throw;
	}
}

connection::~connection()
{
	mysql_close(mysql);
}

statement* connection::prepare(const std::string& query)
{
	return new statement(mysql, query);
}

} // namespace mysqlpp
