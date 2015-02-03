#include <iostream>

#include <mysql/mysql.h>

#include "mysqlpp_result.hpp"
#include "mysqlpp_statement.hpp"
#include "mysqlpp_connection.hpp"
#include "mysqlpp_pool.hpp"

int main()
{
	try
	{
		mysqlpp::connection* conn = new mysqlpp::connection("127.0.0.1", "root", "1235", "test");

		mysqlpp::statement* stmt;
		stmt = conn->prepare_statement("DROP TABLE IF EXISTS test");
		stmt->execute();

		stmt = conn->prepare_statement("CREATE TABLE test(id INT)");
		stmt->execute();

		//stmt = conn->prepare_statement("INSERT INTO test(id) VALUES (?)");
		//stmt->bind
		//stmt->execute();
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