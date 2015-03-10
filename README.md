mysqlpp
=======

Built with
----------
* CMake 2.8 - Ubuntu Linux 14.04 LTS
* Codeblocks 13.12 - Ubuntu Linux 14.04 LTS
* Visual Studio 2010 Express - Windows 7
* Visual Studio 2013 Professional - Windows 7
* Xcode 6.1 - MacOS X 10.9

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
* cmake . (look, there's a dot :)
* make

Run
---
* ./mysqlpp

Memory Leak Check
-----------------
* sudo apt-get install valgrind
* valgrind ./mysqlpp

For Windows
-----------
* mariadb-10.0.16-winx64.msi (mariadb-10.0.16-win32.msi)

For MacOS X
-----------
* install macports (http://www.macports.org)
* sudo port install mariadb
