//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_CONNECTION_HPP
#define MYSQLPP_CONNECTION_HPP

#include <string>

#include <mysql/mysql.h>

namespace mysqlpp
{

class connection
{
public:
	connection(const std::string& host, const std::string& userid, const std::string& passwd, const std::string& dbname, unsigned int port = 3306);
	~connection();

	statement* prepare_statement(const std::string& query);

private:
	st_mysql* mysql;
};

} // namespace mysqlpp

#endif // MYSQLPP_CONNECTION_HPP
