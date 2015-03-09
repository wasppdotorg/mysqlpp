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

		stmt_ptr stmt2(conn->prepare("CREATE TABLE test(col1 TINYINT, col2 SMALLINT unsigned, col3 INT unsigned, col4 BIGINT unsigned, col5 FLOAT, col6 DOUBLE, col7 VARCHAR(10), col8 TEXT, col9 DATETIME, col10 DATETIME, col11 INT NULL)"));
		stmt2->execute();

		unsigned char param1 = 1;
		unsigned short int param2 = 2;
		unsigned int param3 = 3;
		unsigned long long int param4 = 4;
		float param5 = 5.01f;
		double param6 = 6.01f;

		std::string param7("param7");
		std::string param8("param8");

		mysqlpp::datetime datetime_;
		std::string param9 = datetime_.str();

		stmt_ptr stmt3(conn->prepare("INSERT INTO test(col1, col2, col3, col4, col5, col6, col7, col8, col9, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)"));
		{
			stmt3->param(param1);
			stmt3->param(param2);
			stmt3->param(param3);
			stmt3->param(param4);
			stmt3->param(param5);
			stmt3->param(param6);
			stmt3->param(param7);
			stmt3->param(param8);
			stmt3->param(param9);
			stmt3->param_null();
		}
		stmt3->execute();

		// insert one more time
		unsigned long long affected_rows = stmt3->execute();
		std::cout << affected_rows << " rows affected" << std::endl << std::endl;

		stmt_ptr stmt4(conn->prepare("SELECT col1, col2, col3, col4, col5, col6, col7, col9, col8, col9, col10, col11 from test WHERE col1 = ?"));
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
			std::cout << "col1(0) : " << r1->get<unsigned short int>(0) << std::endl;
			std::cout << "col2(1) : " << r1->get<unsigned short int>(1) << std::endl;

			std::cout << "col1 : " << r1->get<unsigned short int>("col1") << std::endl;
			std::cout << "col2 : " << r1->get<unsigned short int>("col2") << std::endl;
			std::cout << "col3 : " << r1->get<unsigned int>("col3") << std::endl;
			std::cout << "col4 : " << r1->get<unsigned long long int>("col4") << std::endl;
			std::cout << "col5 : " << r1->get<float>("col5") << std::endl;
			std::cout << "col6 : " << r1->get<double>("col6") << std::endl;
			std::cout << "col7 : " << r1->get<std::string>("col7") << std::endl;
			std::cout << "col8 : " << r1->get<std::string>("col8") << std::endl;
			std::cout << "col9 : " << r1->get<std::string>("col9") << std::endl;
			std::cout << "col9 : " << r1->get<std::string>("col10") << std::endl;

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
		if (r2->fetch())
		{
			std::cout << r2->get<unsigned int>("last_key") << std::endl;
		}

		stmt_ptr stmt6(conn->prepare("INSERT INTO test(col1, col2, col3, col4, col5, col6, col7, col8, col9, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)"));
		{
			stmt6->param(param1);
			stmt6->param(param2);
			stmt6->param(param3);
			stmt6->param(param4);
			stmt6->param(param5);
			stmt6->param(param6);
			stmt6->param(param7);
			stmt6->param(param8);
			stmt6->param(param9);
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
