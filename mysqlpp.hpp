/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef mysqlpp_hpp
#define mysqlpp_hpp

#include <ctime>

#include <mysql/mysql.h>

#include <stdexcept>
#include <sstream>
#include <vector>

namespace mysqlpp
{

	class exception : public std::runtime_error
	{
	public:
		exception(const char* file, int line, const std::string& what) : std::runtime_error(what)
		{
			std::ostringstream oss;
			oss << file << ":" << line << " " << what;
			err = oss.str();
		}

		~exception() throw() {}

		const char* what() const throw()
		{
			return err.c_str();
		}

	private:
		std::string err;

	};

	struct datetime : st_mysql_time
	{
		datetime()
		{
			auto time_ = std::time(nullptr);
			auto tm_ = *std::localtime(&time_);

			set_time(tm_);
		}

		datetime(const std::string& str)
		{
			if (str.size() > 19)
			{
				throw exception(__FILE__, __LINE__, "datetime cast failed");
			}

			std::tm tm_;
			auto count_ = std::sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
			if (count_ != 3 && count_ != 6)
			{
				throw exception(__FILE__, __LINE__, "datetime cast failed");
			}

			tm_.tm_year -= 1900;
			tm_.tm_mon -= 1;
			tm_.tm_isdst = -1;

			if (std::mktime(&tm_) == -1)
			{
				throw exception(__FILE__, __LINE__, "datetime cast failed");
			}

			set_time(tm_);
		}

		void set_time(const std::tm& tm_)
		{
			year = static_cast<unsigned int>(tm_.tm_year) + 1900;
			month = static_cast<unsigned int>(tm_.tm_mon) + 1;
			day = static_cast<unsigned int>(tm_.tm_mday);
			hour = static_cast<unsigned int>(tm_.tm_hour);
			minute = static_cast<unsigned int>(tm_.tm_min);
			second = static_cast<unsigned int>(tm_.tm_sec);
		}

		std::tm c_tm()
		{
			std::tm tm_;

			tm_.tm_year = static_cast<int>(year) - 1900;
			tm_.tm_mon = static_cast<int>(month) - 1;
			tm_.tm_mday = static_cast<int>(day);
			tm_.tm_hour = static_cast<int>(hour);
			tm_.tm_min = static_cast<int>(minute);
			tm_.tm_sec = static_cast<int>(second);
			tm_.tm_isdst = -1;

			return tm_;
		}

		std::string str()
		{
			char buf[32] = { 0 };

			auto tm_ = c_tm();
			std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_);

			return std::string(buf);
		}
	};

	struct st_mysql_column
	{
		st_mysql_column() : buffer(0), length(0), is_unsigned_(0), is_null(0), error(0)
		{
		}

		enum_field_types type;
		char* name;

		std::vector<char> buffer;
		unsigned long length;

		char is_unsigned_;
		char is_null;
		char error;
	};

	class result
	{
	public:
		result(st_mysql_stmt* stmt_);
		~result();

		bool bind();
		uint64_t num_rows();
		bool fetch(bool is_proc = false);

		template<typename T>
		T get(unsigned int index)
		{
			auto column = get_column(index);
			if (column.is_null)
			{
				throw exception(__FILE__, __LINE__, "null value field");
			}

			T value;
			set_value(column, value);

			return value;
		}

		template<typename T>
		T get(const std::string& name)
		{
			auto column = get_column(name);
			if (column.is_null)
			{
				throw exception(__FILE__, __LINE__, "null value field");
			}

			T value;
			set_value(column, value);

			return value;
		}

		bool is_null(unsigned int index);
		bool is_null(const std::string& name);

	private:
		bool fetch_stmt_result();
		bool fetch_proc_result();

		st_mysql_column& get_column(unsigned int index);
		st_mysql_column& get_column(const std::string& name);

		void set_value(const st_mysql_column& column, uint8_t& value);

		void set_value(const st_mysql_column& column, int16_t& value);
		void set_value(const st_mysql_column& column, uint16_t& value);

		void set_value(const st_mysql_column& column, int32_t& value);
		void set_value(const st_mysql_column& column, uint32_t& value);

		void set_value(const st_mysql_column& column, int64_t& value);
		void set_value(const st_mysql_column& column, uint64_t& value);

		void set_value(const st_mysql_column& column, float& value);
		void set_value(const st_mysql_column& column, double& value);

		void set_value(const st_mysql_column& column, std::string& value);

		st_mysql_stmt* stmt;
		st_mysql_res* metadata;
		st_mysql_field* fields;

		unsigned int field_count;

		std::vector<st_mysql_bind> binds;
		std::vector<st_mysql_column> columns;
	};

	class statement
	{
	public:
		statement(st_mysql* mysql, const std::string& query);
		~statement();

		void param(const uint8_t& value);

		void param(const int16_t& value);
		void param(const uint16_t& value);

		void param(const int32_t& value);
		void param(const uint32_t& value);

		void param(const int64_t& value);
		void param(const uint64_t& value);

		void param(const float& value);
		void param(const double& value);

		void param(const std::string& value);
		void param_blob(const std::string& value);
		void param_null(char is_null = 1);

		uint64_t execute();
		result* query();

	private:
		st_mysql_bind& get_bind();

		st_mysql_stmt* stmt;

		int param_count;
		int bind_index;

		std::vector<st_mysql_bind> binds;
		std::vector<unsigned long> lengths;
	};

	class connection
	{
	public:
		connection(const std::string& host, const std::string& userid, const std::string& passwd, const std::string& database, unsigned int port = 3306, const std::string& charset = "utf8", bool pooled_ = true);
		~connection();

		bool ping();
		statement* prepare(const std::string& query);
		statement* prepare_like(const std::string& query, bool left_percent, const std::string& keyword, bool right_percent);

		std::tm* last_released()
		{
			return &released;
		}

		void set_released(const std::tm& released_)
		{
			released = released_;
		}

		bool is_pooled()
		{
			return pooled;
		}

		void set_pooled(bool pooled_)
		{
			pooled = pooled_;
		}

	private:
		st_mysql* mysql;

		std::tm released;
		bool pooled;

	};

} // namespace mysqlpp

#endif // mysqlpp_hpp
