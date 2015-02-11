
#include "mysqlpp.hpp"

int main()
{
	try
	{
		mysqlpp::connection* conn = new mysqlpp::connection("127.0.0.1", "root", "1235", "test");

		mysqlpp::statement* stmt;
		stmt = conn->prepare("DROP TABLE IF EXISTS test");
		stmt->execute();

		stmt = conn->prepare("CREATE TABLE test(col1 TINYINT, col2 SMALLINT, col3 INT, col4 BIGINT, col5 FLOAT, col6 DOUBLE, col7 VARCHAR(10), col8 TEXT, col9 DATETIME, col10 DATETIME, col11 INT NULL)");
		stmt->execute();

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

		stmt = conn->prepare("INSERT INTO test(col1, col2, col3, col4, col5, col6, col7, col8, col9, col10, col11) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)");
		{
			stmt->param(param1);
			stmt->param(param2);
			stmt->param(param3);
			stmt->param(param4);
			stmt->param(param5);
			stmt->param(param6);
			stmt->param(param7);
			stmt->param(param8);
			stmt->param(param9);
			stmt->param_null();
		}
		stmt->execute();

		// insert one more time
		unsigned long long affected_rows = stmt->execute();
		std::cout << affected_rows << " rows affected" << std::endl << std::endl;

		stmt = conn->prepare("SELECT col1, col2, col3, col4, col5, col6, col7, col9, col8, col9, col10, col11 from test");
		mysqlpp::result* r = stmt->query();

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
