mysqlpp
=======

Tested with
-----------
* CMake on Ubuntu 14.04 LTS Linux Server (Boost 1.54)
* Visual Studio 2010 Express on Windows 7 (Boost 1.54 lib32-msvc-10.0)
* Visual Studio 2013 Professional on Windows 7 (Boost 1.55 lib64-msvc-12.0)


Install MariaDB
---------------
* sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 0xcbcb082a1bb943db
* sudo vi /etc/apt/sources.list
```
deb http://ftp.osuosl.org/pub/mariadb/repo/10.0/ubuntu trusty main
deb-src http://ftp.osuosl.org/pub/mariadb/repo/10.0/ubuntu trusty main
```

* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo vi /etc/mysql/conf.d/mariadb.cnf
```
[client]
default-character-set = utf8

[mysqld]
character-set-server = utf8
collation-server     = utf8_general_ci
character_set_server = utf8
collation_server     = utf8_general_ci
```

* sudo /etc/init.d/mysql restart
* mysql -u root -p
* create database test default character set utf8 default collate utf8_general_ci;
* exit

Install MySQLClient Library
---------------------------
* sudo apt-get install libmariadbclient-dev

Install mysqlpp
-------------
* sudo apt-get install git
* git clone https://github.com/waspporg/mysqlpp.git
* cd mysqpp
* sudo apt-get install build-essential
* sudo apt-get install cmake
* cmake .
* make

Run
---
* ./mysqlpp

Check
-----
* sudo apt-get install valgrind
* valgrind ./mysqlpp
