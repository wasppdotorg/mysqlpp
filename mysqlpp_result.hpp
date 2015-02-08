//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_RESULT_HPP
#define MYSQLPP_RESULT_HPP

#include <string>
#include <vector>

#include <mysql/mysql.h>

#include "mysqlpp_exception.hpp"
#include "mysqlpp_result.hpp"

namespace mysqlpp
{

struct st_mysql_column
{
	st_mysql_column() : buffer(0), length(0), is_null(0), error(0)
	{
	}

	enum_field_types type;
	std::string name;

	std::vector<char> buffer;
	unsigned long length;

	my_bool is_null;
	my_bool error;
};

class result
{
public:
	result(st_mysql_stmt* stmt_);
	~result();

	unsigned long long num_rows();
	bool fetch();


	template<typename T>
	T field(const std::string& name)
	{
		T value = T();
		if (!fetch_column(index(name), value))
		{
			throw exception("null value fetch");
		}

		return value;
	}

	template<typename T>
	T field(unsigned int index)
	{
		T value = T();
		if (!fetch_column(index, value))
		{
			throw exception("null value fetch");
		}

		return value;
	}

	template<typename T>
	T field(unsigned int index)
	{
		st_mysql_column& column = this_column(index);

		return static_cast<T>((char&)column.buffer.front());
	}
	
	template<typename T>
	T field(const std::string& name)
	{
		st_mysql_column& column = this_column(name);

		return static_cast<T>((char&)column.buffer.front());
	}
	
	/*

	short int field(unsigned int index)
	{
		st_mysql_column& column = this_column(index);

		return static_cast<short int>((char&)column.buffer.front());
	}

	int field(const std::string& name)
	{
		st_mysql_column& column = this_column(name);

		return static_cast<int>((char&)column.buffer.front());
	}

	
	float field(unsigned int index)
	{
		st_mysql_column& column = this_column(index);

		return *reinterpret_cast<float*>((char*)column.buffer.front());
	}

	float field(const std::string& name)
	{
		st_mysql_column& column = this_column(name);

		return *reinterpret_cast<float*>((char*)column.buffer.front());
	}
	*/

	/*
	double field(unsigned int index, char* end)
	{
		st_mysql_column& column = this_column(index);

		return *reinterpret_cast<double*>((char*)column.buffer.front());
	}

	double field(const std::string& name, char* end)
	{
		st_mysql_column& column = this_column(name);

		return *reinterpret_cast<double*>((char*)column.buffer.front());
	}

	std::string field(unsigned int index)
	{
		st_mysql_column& column = this_column(index);

		return std::string((char*)column.buffer.front());
	}

	/*
	st_mysql_time field(unsigned int index)
	{
		return st_mysql_time();
	}

	st_mysql_time field(const std::string& name)
	{
		return st_mysql_time();
	}
	*/

	//int field(unsigned int index);
	//int field(const std::string& name);

	/*
	template<typename T>
	T field(const std::string& name)
	{
		T value = T();
		if (!fetch(name, value))
		{
			throw exception("null value fetch");
		}

		return value;
	}

	template<typename T>
	T field(int index)
	{
		T value = T();
		if (!fetch(index, value))
		{
			throw exception("null value fetch");
		}

		return value;
	}
	*/

private:
	/*
	std::string name(int index);
	int index(const std::string& name);
	*/

	unsigned int index(const std::string& name);

	bool fetch_column(unsigned int index, short int &value);
	bool fetch_column(unsigned int index, unsigned short int &value);
	bool fetch_column(unsigned int index, int &value);
	bool fetch_column(unsigned int index, unsigned int &value);
	bool fetch_column(unsigned int index, long int &value);
	bool fetch_column(unsigned int index, unsigned long int &value);
	bool fetch_column(unsigned int index, long long int &value);
	bool fetch_column(unsigned int index, unsigned long long int &value);
	bool fetch_column(unsigned int index, float &value);
	bool fetch_column(unsigned int index, double &value);
	bool fetch_column(unsigned int index, long double &value);

	bool fetch_column(unsigned int index, std::string &value);
	bool fetch_column(unsigned int index, std::tm& value);
	//bool fetch(int index, std::ostream &value);

	/*
	bool fetch(const std::string& name, short int &value);
	bool fetch(const std::string& name, unsigned short int &value);
	bool fetch(const std::string& name, int &value);
	bool fetch(const std::string& name, unsigned int &value);
	bool fetch(const std::string& name, long int &value);
	bool fetch(const std::string& name, unsigned long int &value);
	bool fetch(const std::string& name, long long int &value);
	bool fetch(const std::string& name, unsigned long long int &value);
	bool fetch(const std::string& name, float &value);
	bool fetch(const std::string& name, double &value);
	bool fetch(const std::string& name, long double &value);

	bool fetch(const std::string& name, std::tm& value);
	bool fetch(const std::string& name, std::string &value);
	bool fetch(const std::string& name, std::ostream &value);
	*/

	st_mysql_column& this_column(unsigned int index)
	{
		if (index < 0 || index >= field_count)
		{
			throw exception("invalid column_index");
		}

		return columns.at(index);
	}

	st_mysql_column& this_column(const std::string& name)
	{
		unsigned int index = 0;
		for (; index < field_count; ++index)
		{
			if (name == columns[index].name)
			{
				break;
			}
		}

		if (index == field_count)
		{
			throw exception("invalid column_name");
		}

		return columns.at(index);
	}

	/*
	template<typename T> bool fetch_data(int index, T& value)
	{
		mysqlpp_data& dat = data_at(index);
		if (dat.is_null)
		{
			return false;
		}

		std::string str(dat.ptr, dat.length);

		iss.clear();
		iss.str(str);

		T value_;
		iss >> value_;
		if (iss.fail() || !std::ws(iss).eof())
		{
			throw exception("bad value cast");
		}

		if (std::numeric_limits<T>::is_integer)
		{
			if (!std::numeric_limits<T>::is_signed && str.find('-') != std::string::npos && value_ != 0)
			{
				throw exception("bad value cast");
			}
		}

		value = static_cast<T>(value_);
		return true;
	}
	*/

	st_mysql_stmt* stmt;
	st_mysql_res* metadata;
	st_mysql_field* fields;

	unsigned int field_count;

	std::vector<st_mysql_bind> binds;
	std::vector<st_mysql_column> columns;
};

} // namespace mysqlpp

#endif // MYSQLPP_RESULT_HPP
