/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

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
			mysql_ = mysql_init(0);

			if (!mysql_real_connect(mysql_, host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, 0, 0))
			{
				throw exception(__FILE__, __LINE__, mysql_error(mysql_));
			}

			if (mysql_set_character_set(mysql_, charset.c_str()) != 0)
			{
				throw exception(__FILE__, __LINE__, mysql_error(mysql_));
			}
		}
		catch (...)
		{
			mysql_close(mysql_);

			// let your own database manager or main call this function below
			//mysql_library_end();

			throw;
		}

		auto time_ = std::time(nullptr);
		set_released(*std::localtime(&time_));
		set_pooled(pooled_);
	}

	connection::~connection()
	{
		clear();
		mysql_close(mysql_);

		// let your own database manager or main call this function below
		//mysql_library_end();
	}

	bool connection::ping()
	{
		return mysql_ping(mysql_) == 0;
	}

	statement* connection::prepare(const std::string& query)
	{
		auto stmt = new statement(this, query);
		statements.push_back(stmt);
		
		return stmt;
	}

	statement* connection::prepare_like(const std::string& query, bool left_percent, const std::string& keyword, bool right_percent)
	{
		auto escaped_keyword = new char[(keyword.size() * 2) + 1];

		mysql_real_escape_string(mysql_, escaped_keyword, keyword.c_str(), keyword.size());

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

		auto stmt = new statement(this, oss.str());
		statements.push_back(stmt);
		
		return stmt;
	}
	
	result* connection::__query(st_mysql_stmt* stmt)
	{
		auto rs = new result(stmt);
		results.push_back(rs);

		return rs;
	}

	void connection::clear()
	{
		for (auto& r : results)
		{
			delete r;
		}
		results.clear();
		
		for (auto& s : statements)
		{
			delete s;
		}
		statements.clear();
	}

} // namespace mysqlpp
