//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "mysqlpp.hpp"

namespace mysqlpp
{

	connection::connection(const std::string& host, const std::string& userid, const std::string& passwd, const std::string& database, unsigned int port, bool pooled_)
	{
		mysql = mysql_init(0);

		try
		{
			if (!mysql)
			{
				throw exception("mysql_init failed");
			}

			if (!mysql_real_connect(mysql, host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, 0, 0))
			{
				throw exception(mysql_error(mysql));
			}
		}
		catch (...)
		{
			mysql_close(mysql);
			mysql_library_end();

			throw;
		}

		std::time_t time_ = std::time(0);
		set_released(*std::localtime(&time_));
		set_pooled(pooled_);
	}

	connection::~connection()
	{
		mysql_close(mysql);
		mysql_library_end();
	}

	statement* connection::prepare(const std::string& query)
	{
		return new statement(mysql, query);
	}

} // namespace mysqlpp
