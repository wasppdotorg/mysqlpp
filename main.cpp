
#include <iostream>

#include <mysql/mysql.h>

#include "mysqlpp_result.hpp"
#include "mysqlpp_statement.hpp"
#include "mysqlpp_connection.hpp"

int main()
{
	try
	{
		mysqlpp::connection* conn = new mysqlpp::connection("127.0.0.1", "root", "1235", "test");

		mysqlpp::statement* stmt;
		stmt = conn->prepare("DROP TABLE IF EXISTS test");
		stmt->execute();

		stmt = conn->prepare("CREATE TABLE test(col1 TINYINT, col2 SMALLINT, col3 INT, col4 BIGINT, col5 FLOAT, col6 DOUBLE, col7 VARCHAR(10), col10 INT NULL)");
		stmt->execute();

		stmt = conn->prepare("INSERT INTO test(col1, col2, col3, col4, col5, col6, col7, col10) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
		{
			stmt->param(unsigned char(1));
			stmt->param(short int(2));
			stmt->param(int(3));
			stmt->param(long long int(4));
			stmt->param(float(5));
			stmt->param(double(6));

			stmt->param(new std::string("ÇÑ±Û"));
			//stmt->param(std::string("8"));

			/*
			st_mysql_time* now = new st_mysql_time;
			now->year = 1970;
			now->month = 1;
			now->day = 1;
			now->hour = 0;
			now->minute = 0;
			now->second = 0;

			stmt->param(now);
			*/
			stmt->param_null();
		}
		stmt->execute();

		//stmt = conn->prepare_statement("SELECT * from test");
		//mysqlpp::result* rs = stmt->query();

		//if (rs->next())
		//{
			//std::cout << 1 << std::endl;
		//}

		//while (rs->next())
		//{
			//std::cout << rs->get<int>(0) << std::endl;
			//std::cout << rs->get<int>("id2") << std::endl;
		//}
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
