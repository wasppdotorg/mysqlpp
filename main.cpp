
#include <iostream>
#include <string>

#include <mysql/mysql.h>

#include "mysqlpp.hpp"

int main()
{
	try
	{
		mysqlpp::connection* conn = new mysqlpp::connection("127.0.0.1", "root", "1235", "test");

		mysqlpp::statement* stmt;
		stmt = conn->prepare("DROP TABLE IF EXISTS test");
		stmt->execute();

		stmt = conn->prepare("CREATE TABLE test(col1 TINYINT, col2 SMALLINT, col3 INT, col4 BIGINT, col5 FLOAT, col6 DOUBLE, col7 VARCHAR(10), col8 TEXT, col9 INT NULL, col10 DATETIME, col11 DATETIME)");
		stmt->execute();

		unsigned char param1 = 1;
		short int param2 = 2;
		int param3 = 3;
		long long int param4 = 4;
		float param5 = 5.01f;
		double param6 = 6.01f;

		std::string param7("param7");
		// std::size_to to unsigned long
		unsigned long param7_size = param7.size();

		std::string param8("param8");
		unsigned long param8_size = param8.size();

		mysqlpp::datetime param10;
		{
			param10.year = 1970;
			param10.month = 1;
			param10.day = 1;
			param10.hour = 0;
			param10.minute = 0;
			param10.second = 0;
		}

		stmt = conn->prepare("INSERT INTO test(col1, col2, col3, col4, col5, col6, col7, col8, col9, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NOW())");
		{
			stmt->param(param1);
			stmt->param(param2);
			stmt->param(param3);
			stmt->param(param4);
			stmt->param(param5);
			stmt->param(param6);
			stmt->param(param7, param7_size);
			stmt->param(param8, param8_size);
			stmt->param_null();
			stmt->param(param10);			
		}
		stmt->execute();

		// insert one more time
		unsigned long long affected_rows = stmt->execute();
		std::cout << affected_rows << " rows affected" << std::endl << std::endl;

		stmt = conn->prepare("SELECT col1, col2, col3, col4, col5, col6, col7, col8, col9 from test");
		mysqlpp::result* r = stmt->query();

		if (r->num_rows() == 0)
		{
			std::cout << "no result" << std::endl << std::endl;
		}

		while (r->fetch())
		{
			std::cout << "param1 : " << r->field<short int>(0) << std::endl;

			std::cout << "param1 : " << r->field<short int>("col1") << std::endl;
			std::cout << "param2 : " << r->field<short int>("col2") << std::endl;
			std::cout << "param3 : " << r->field<int>("col3") << std::endl;
			std::cout << "param4 : " << r->field<long long int>("col4") << std::endl;
			std::cout << "param5 : " << r->field<float>("col5") << std::endl;
			std::cout << "param6 : " << r->field<double>("col6") << std::endl;
			std::cout << "param7 : " << r->field<std::string>("col7") << std::endl;
			std::cout << "param8 : " << r->field<std::string>("col8") << std::endl;
			//std::cout << "param9 : " << r->field<int>("col9") << std::endl;

			//mysqlpp::datetime col10 = r->field<mysqlpp::datetime>("col10");
			//std::cout << "param10 : " << r->time_to_str(col10) << std::endl;

			//mysqlpp::datetime col11 = r->field<mysqlpp::datetime>("col11");
			//std::cout << "param11 : " << r->datetime_str(col11) << std::endl;

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
