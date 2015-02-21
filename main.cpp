
#include <iostream>
#include <memory>

#include "mysqlpp.hpp"

// yeah, right. std::auto_ptr is deprecated
typedef std::auto_ptr<mysqlpp::connection> conn_ptr;
typedef std::auto_ptr<mysqlpp::statement> stmt_ptr;
typedef std::auto_ptr<mysqlpp::result> res_ptr;

// so, if boost is available, then:
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

		stmt_ptr stmt2(conn->prepare("CREATE TABLE test(col1 TINYINT, col2 SMALLINT, col3 INT, col4 BIGINT, col5 FLOAT, col6 DOUBLE, col7 VARCHAR(10), col8 TEXT, col9 DATETIME, col10 DATETIME, col11 INT NULL)"));
		stmt2->execute();

		unsigned char param1 = 1;
		short int param2 = 2;
		int param3 = 3;
		long long int param4 = 4;
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

		stmt_ptr stmt4(conn->prepare("SELECT col1, col2, col3, col4, col5, col6, col7, col9, col8, col9, col10, col11 from test"));
		res_ptr r(stmt4->query());

		if (r->num_rows() == 0)
		{
			std::cout << "no result" << std::endl << std::endl;
		}

		while (r->fetch())
		{
			std::cout << "col1(0) : " << r->get<short int>(0) << std::endl;
			std::cout << "col2(1) : " << r->get<short int>(1) << std::endl;

			std::cout << "col1 : " << r->get<short int>("col1") << std::endl;
			std::cout << "col2 : " << r->get<short int>("col2") << std::endl;
			std::cout << "col3 : " << r->get<int>("col3") << std::endl;
			std::cout << "col4 : " << r->get<long long int>("col4") << std::endl;
			std::cout << "col5 : " << r->get<float>("col5") << std::endl;
			std::cout << "col6 : " << r->get<double>("col6") << std::endl;
			std::cout << "col7 : " << r->get<std::string>("col7") << std::endl;
			std::cout << "col8 : " << r->get<std::string>("col8") << std::endl;
			std::cout << "col9 : " << r->get<std::string>("col9") << std::endl;
			std::cout << "col9 : " << r->get<std::string>("col10") << std::endl;

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
