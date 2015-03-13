/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "mysqlpp.hpp"

namespace mysqlpp
{

	connection::connection(const std::string& host, const std::string& userid, const std::string& passwd, const std::string& database, unsigned int port, const std::string& charset, bool pooled_)
	{
		mysql = mysql_init(0);

		try
		{
			if (!mysql)
			{
				throw exception(__FILE__, __LINE__, "mysql_init failed");
			}

			if (!mysql_real_connect(mysql, host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, 0, 0))
			{
				throw exception(__FILE__, __LINE__, mysql_error(mysql));
			}

			if (mysql_set_character_set(mysql, charset.c_str()) != 0)
			{
				throw exception(__FILE__, __LINE__, mysql_error(mysql));
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
