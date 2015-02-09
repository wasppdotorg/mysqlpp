//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_STATEMENT_HPP
#define MYSQLPP_STATEMENT_HPP

#include <vector>
#include <string>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_result.hpp"

namespace mysqlpp
{

	class statement
	{
	public:
		statement(st_mysql* mysql, const std::string& query);
		~statement();

		void param(const unsigned char& value);
		void param(const short int& value);
		void param(const int& value);
		void param(const long long int& value);
		void param(const float& value, unsigned long length = 0);
		void param(const double& value, unsigned long length = 0);
		void param(const std::string& value, unsigned long& length);
		void param(const st_mysql_time& value);
		void param_null(my_bool is_null = 1);

		unsigned long long execute();
		result* query();

	private:
		st_mysql_bind& this_bind()
		{
			if (bind_index < 0 || bind_index >= param_count)
			{
				throw exception("invalid bind_index");
			}

			return binds[bind_index++];
		}

		st_mysql_stmt* stmt;

		int param_count;
		int bind_index;

		std::vector<st_mysql_bind> binds;
	};

} // namespace mysqlpp

#endif // MYSQLPP_STATEMENT_HPP
