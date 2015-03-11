/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <iostream>
#include <memory>

#include "mysqlpp.hpp"

typedef std::auto_ptr<mysqlpp::connection> conn_ptr;
typedef std::auto_ptr<mysqlpp::statement> stmt_ptr;
typedef std::auto_ptr<mysqlpp::result> res_ptr;

// std::auto_ptr is deprecated.
// if boost is available, then:
/*
#include <boost/scoped_ptr.hpp>

typedef boost::scoped_ptr<mysqlpp::connection> conn_ptr;
typedef boost::scoped_ptr<mysqlpp::statement> stmt_ptr;
typedef boost::scoped_ptr<mysqlpp::result> res_ptr;
*/

int main()
{
	try
	{
		conn_ptr conn(new mysqlpp::connection("127.0.0.1", "root", "1235", "test"));

		stmt_ptr stmt(conn->prepare("DROP TABLE IF EXISTS test"));
		stmt->execute();

		stmt.reset(conn->prepare("CREATE TABLE test(col01 TINYINT, col02 SMALLINT unsigned, col03 INT unsigned, col04 BIGINT unsigned, col05 FLOAT, col06 DOUBLE, col07 VARCHAR(10), col08 TEXT, col09 DATETIME, col10 DATETIME, col11 INT NULL)"));
		stmt->execute();

		unsigned char param01 = 1;
		unsigned short int param02 = 2;
		unsigned int param03 = 3;
		unsigned long long int param04 = 4;
		float param05 = 5.01f;
		double param06 = 6.01f;

		std::string param07("param7");
		std::string param08("param8");

		mysqlpp::datetime datetime_;
		std::string param09 = datetime_.str();

		stmt.reset(conn->prepare("INSERT INTO test(col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)"));
		{
			stmt->param(param01);
			stmt->param(param02);
			stmt->param(param03);
			stmt->param(param04);
			stmt->param(param05);
			stmt->param(param06);
			stmt->param(param07);
			stmt->param(param08);
			stmt->param(param09);
			stmt->param_null();
		}
		stmt->execute();

		// insert one more time
		unsigned long long int affected_rows = stmt->execute();
		std::cout << affected_rows << " rows affected" << std::endl << std::endl;

		stmt.reset(conn->prepare("SELECT col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11 from test WHERE col01 = ?"));
		{
			stmt->param(1);
		}

		res_ptr r(stmt->query());
		if (r->num_rows() == 0)
		{
			std::cout << "no result" << std::endl << std::endl;
		}

		while (r->fetch())
		{
			std::cout << "col01(0) : " << r->get<unsigned short int>(0) << std::endl;
			std::cout << "col02(1) : " << r->get<unsigned short int>(1) << std::endl;

			std::cout << "col01 : " << r->get<unsigned short int>("col01") << std::endl;
			std::cout << "col02 : " << r->get<unsigned short int>("col02") << std::endl;
			std::cout << "col03 : " << r->get<unsigned int>("col03") << std::endl;
			std::cout << "col04 : " << r->get<unsigned long long int>("col04") << std::endl;
			std::cout << "col05 : " << r->get<float>("col05") << std::endl;
			std::cout << "col06 : " << r->get<double>("col06") << std::endl;
			std::cout << "col07 : " << r->get<std::string>("col07") << std::endl;
			std::cout << "col08 : " << r->get<std::string>("col08") << std::endl;
			std::cout << "col09 : " << r->get<std::string>("col09") << std::endl;
			std::cout << "col10 : " << r->get<std::string>("col10") << std::endl;

			if (r->is_null("col11"))
			{
				std::cout << "param11 is null" << std::endl;
			}
			else
			{
				std::cout << "param11 : " << r->get<int>("col11") << std::endl;
			}

			std::cout << "--" << std::endl;
		}

		/*
		stmt.reset(conn->prepare("CALL USP_GET_UNIQUE_KEYS('users_idx', ?)"));
		{
			stmt->param(1);
		}

		r.reset(stmt->query());
		if (r->fetch(true))
		{
			std::cout << r->get<int>("last_key") << std::endl;
		}
		*/

		stmt.reset(conn->prepare("INSERT INTO test(col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)"));
		{
			stmt->param(param01);
			stmt->param(param02);
			stmt->param(param03);
			stmt->param(param04);
			stmt->param(param05);
			stmt->param(param06);
			stmt->param(param07);
			stmt->param(param08);
			stmt->param(param09);
			stmt->param_null();
		}
		stmt->execute();

		// insert one more time
		affected_rows = stmt->execute();
		std::cout << affected_rows << " rows affected" << std::endl << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;

		std::cin.get();
		return 1;
	}

	std::cout << "OK" << std::endl;

	std::cin.get();
	return 0;
}
