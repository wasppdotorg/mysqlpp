mysqlpp
=======

Build Environment
-----------------
* GCC 4.8 - Ubuntu Linux 14.04 LTS
* Clang 3.4 - FreeBSD 10.2
* Visual Studio 2013 - Windows 7

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
```
create database test default character set utf8 default collate utf8_general_ci;
exit
```

Install MariaDB Client Library
-----------------------------
* sudo apt-get install libmariadbclient-dev

Install mysqlpp
-------------
* sudo apt-get install git
* git clone https://github.com/waspporg/mysqlpp.git
* cd mysqpp
* sudo apt-get install build-essential
* sudo apt-get install cmake
* mkdir cmakebuild
* cd cmakebuild
* cmake ..
* make

Run
---
* ./mysqlpp

Memory Leak Check
-----------------
* sudo apt-get install valgrind
* valgrind ./mysqlpp

For FreeBSD
-----------
* sudo pkg install mariadb100-client
* sudo pkg install mariadb100-server

For Windows
-----------
* mariadb-10.0.22-winx64.msi
