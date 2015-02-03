//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_MYSQL_EXCEPT_HPP
#define WASPP_MYSQL_EXCEPT_HPP

#include <string>
#include <stdexcept>

namespace waspp
{

class mysql_exception : public std::runtime_error
{
public:
	mysql_exception(const std::string& what_arg) : std::runtime_error(what_arg)
	{
		
	}
};

} // namespace waspp

#endif // WASPP_MYSQL_EXCEPT_HPP
