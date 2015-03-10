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

		stmt_ptr stmt1(conn->prepare("DROP TABLE IF EXISTS test"));
		stmt1->execute();

		stmt_ptr stmt2(conn->prepare("CREATE TABLE test(col01 TINYINT, col02 SMALLINT unsigned, col03 INT unsigned, col04 BIGINT unsigned, col05 FLOAT, col06 DOUBLE, col07 VARCHAR(10), col08 TEXT, col09 DATETIME, col10 DATETIME, col11 INT NULL)"));
		stmt2->execute();

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

		stmt_ptr stmt3(conn->prepare("INSERT INTO test(col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)"));
		{
			stmt3->param(param01);
			stmt3->param(param02);
			stmt3->param(param03);
			stmt3->param(param04);
			stmt3->param(param05);
			stmt3->param(param06);
			stmt3->param(param07);
			stmt3->param(param08);
			stmt3->param(param09);
			stmt3->param_null();
		}
		stmt3->execute();

		// insert one more time
		unsigned long long int affected_rows = stmt3->execute();
		std::cout << affected_rows << " rows affected" << std::endl << std::endl;

		stmt_ptr stmt4(conn->prepare("SELECT col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11 from test WHERE col01 = ?"));
		{
			stmt4->param(1);
		}

		res_ptr r1(stmt4->query());
		if (r1->num_rows() == 0)
		{
			std::cout << "no result" << std::endl << std::endl;
		}

		while (r1->fetch())
		{
			std::cout << "col01(0) : " << r1->get<unsigned short int>(0) << std::endl;
			std::cout << "col02(1) : " << r1->get<unsigned short int>(1) << std::endl;

			std::cout << "col01 : " << r1->get<unsigned short int>("col01") << std::endl;
			std::cout << "col02 : " << r1->get<unsigned short int>("col02") << std::endl;
			std::cout << "col03 : " << r1->get<unsigned int>("col03") << std::endl;
			std::cout << "col04 : " << r1->get<unsigned long long int>("col04") << std::endl;
			std::cout << "col05 : " << r1->get<float>("col05") << std::endl;
			std::cout << "col06 : " << r1->get<double>("col06") << std::endl;
			std::cout << "col07 : " << r1->get<std::string>("col07") << std::endl;
			std::cout << "col08 : " << r1->get<std::string>("col08") << std::endl;
			std::cout << "col09 : " << r1->get<std::string>("col09") << std::endl;
			std::cout << "col10 : " << r1->get<std::string>("col10") << std::endl;

			if (r1->is_null("col11"))
			{
				std::cout << "param11 is null" << std::endl;
			}
			else
			{
				std::cout << "param11 : " << r1->get<int>("col11") << std::endl;
			}

			std::cout << "--" << std::endl;
		}
		
		stmt_ptr stmt5(conn->prepare("CALL USP_GET_UNIQUE_KEYS(?, ?)"));
		{
			stmt5->param("users_idx");
			stmt5->param(1);
		}

		res_ptr r2(stmt5->query());
		if (r2->fetch_proc_result())
		{
			std::cout << 1 << std::endl;
			std::cout << r2->get<unsigned int>("last_key") << std::endl;
			std::cout << 2 << std::endl;
		}

		stmt_ptr stmt6(conn->prepare("INSERT INTO test(col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)"));
		{
			stmt6->param(param01);
			stmt6->param(param02);
			stmt6->param(param03);
			stmt6->param(param04);
			stmt6->param(param05);
			stmt6->param(param06);
			stmt6->param(param07);
			stmt6->param(param08);
			stmt6->param(param09);
			stmt6->param_null();
		}
		stmt6->execute();

		// insert one more time
		affected_rows = stmt6->execute();
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
