/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <cstdlib>

#include <memory>
#include <iostream>

#include "mysqlpp.hpp"

using conn_ptr = std::unique_ptr<mysqlpp::connection>;
using stmt_ptr = std::unique_ptr<mysqlpp::statement>;
using rs_ptr = std::unique_ptr<mysqlpp::result>;

int main()
{
	try
	{
		mysql_library_init(0, 0, 0);
		atexit(mysql_library_end);

		conn_ptr conn(new mysqlpp::connection("127.0.0.1", "dbuser", "passwd", "test"));

		stmt_ptr stmt(conn->prepare("DROP TABLE IF EXISTS test"));
		stmt->execute();

		stmt.reset(conn->prepare("CREATE TABLE test(col01 TINYINT unsigned, col02 SMALLINT unsigned, col03 INT unsigned, col04 BIGINT unsigned, col05 FLOAT, col06 DOUBLE, col07 VARCHAR(10), col08 TEXT, col09 BLOB, col10 DATETIME, col11 INT NULL, col12 DATETIME)"));
		stmt->execute();

		uint8_t param01 = 255;
		uint16_t param02 = 65535;
		uint32_t param03 = 429496729;//5;
		uint64_t param04 = 1844674407370955161;//5;
		float param05 = 5.01f;
		double param06 = 6.01f;

		std::string param07("param7");
		std::string param08("param8");
		std::string param09("param9");

		mysqlpp::datetime datetime_;
		std::string param10 = datetime_.str();

		stmt.reset(conn->prepare("INSERT INTO test(col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NOW())"));
		//
			stmt->param(param01);
			stmt->param(param02);
			stmt->param(param03);
			stmt->param(param04);
			stmt->param(param05);
			stmt->param(param06);
			stmt->param(param07);
			stmt->param(param08);
			stmt->param(param09);
			stmt->param(param10);
			stmt->param_null();
		//
		stmt->execute();

		// insert one more time
		uint64_t affected_rows = stmt->execute();
		std::cout << affected_rows << " rows affected" << std::endl << std::endl;

		stmt.reset(conn->prepare("SELECT col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12 from test WHERE col01 = ?"));
		//
			stmt->param(255);
		//

		//stmt.reset(conn->prepare_like("SELECT col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12 from test WHERE col07 like ", false, "param", true));

		rs_ptr rs(stmt->query());
		if (rs->num_rows() == 0)
		{
			std::cout << "no result" << std::endl << std::endl;
		}

		while (rs->fetch())
		{
			std::cout << "col01(0) : " << (int)rs->get<uint8_t>(0) << std::endl;
			std::cout << "col02(1) : " << rs->get<uint16_t>(1) << std::endl;

			std::cout << "col01 : " << (int)rs->get<uint8_t>("col01") << std::endl;
			std::cout << "col02 : " << rs->get<uint16_t>("col02") << std::endl;
			std::cout << "col03 : " << rs->get<uint32_t>("col03") << std::endl;
			std::cout << "col04 : " << rs->get<uint64_t>("col04") << std::endl;
			std::cout << "col05 : " << rs->get<float>("col05") << std::endl;
			std::cout << "col06 : " << rs->get<double>("col06") << std::endl;
			std::cout << "col07 : " << rs->get<std::string>("col07") << std::endl;
			std::cout << "col08 : " << rs->get<std::string>("col08") << std::endl;
			std::cout << "col09 : " << rs->get<std::string>("col09") << std::endl;
			std::cout << "col10 : " << rs->get<std::string>("col10") << std::endl;

			if (rs->is_null("col11"))
			{
				std::cout << "col11 is null" << std::endl;
			}
			else
			{
				std::cout << "col11 : " << rs->get<int32_t>("col11") << std::endl;
			}

			std::cout << "col12 : " << rs->get<std::string>("col12") << std::endl;
			std::cout << "--" << std::endl;
		}

		/*
		stmt.reset(conn->prepare("CALL USP_GET_UNIQUE_KEYS('users', ?)"));
		//
			stmt->param(1);
		//

		rs.reset(stmt->query());
		if (rs->fetch(true))
		{
			std::cout << rs->get<uint64_t>("last_key") << std::endl;
		}
		*/

		stmt.reset(conn->prepare("INSERT INTO test(col01, col02, col03, col04, col05, col06, col07, col08, col09, col10, col11, col12) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NOW())"));
		//
			stmt->param(param01);
			stmt->param(param02);
			stmt->param(param03);
			stmt->param(param04);
			stmt->param(param05);
			stmt->param(param06);
			stmt->param(param07);
			stmt->param(param08);
			stmt->param(param09);
			stmt->param(param10);
			stmt->param_null();
		//
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
