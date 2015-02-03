//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MYSQLPP_EXCEPTION_HPP
#define MYSQLPP_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace mysqlpp
{

class exception : public std::runtime_error
{
public:
    exception(const std::string& what) : std::runtime_error(what)
    {

    }
};

} // namespace mysqlpp

#endif // MYSQLPP_EXCEPTION_HPP
