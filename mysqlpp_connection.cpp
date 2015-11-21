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
		try
		{
			mysql = mysql_init(0);

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

			// let your own database manager or main call this function below
			//mysql_library_end();

			throw;
		}

		std::time_t time_ = std::time(0);
		set_released(*std::localtime(&time_));
		set_pooled(pooled_);
	}

	connection::~connection()
	{
		mysql_close(mysql);

		// let your own database manager or main call this function below
		//mysql_library_end();
	}

	bool connection::ping()
	{
		return mysql_ping(mysql) == 0;
	}

	statement* connection::prepare(const std::string& query)
	{
		return new statement(mysql, query);
	}

	statement* connection::prepare_like(const std::string& query, bool left_percent, const std::string& keyword, bool right_percent)
	{
		char* escaped_keyword = new char[(keyword.size() * 2) + 1];

		mysql_real_escape_string(mysql, escaped_keyword, keyword.c_str(), keyword.size());

		std::ostringstream oss;
		oss << query << " '";

		if (left_percent)
		{
			oss << "%";
		}
		oss << escaped_keyword;
		delete[] escaped_keyword;

		if (right_percent)
		{
			oss << "%";
		}
		oss << "'";

		return new statement(mysql, oss.str());
	}

} // namespace mysqlpp
