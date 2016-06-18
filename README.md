
mysqlpp
=======

Build Environment
-----------------
* GCC 4.9 - Debian 8
* Clang 3.4 - FreeBSD 10.2
* Visual Studio 2013 - Windows 7

Install MariaDB
---------------
* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo /etc/init.d/mysql restart
* sudo mysql -u root -p
```
create database test default character set utf8 default collate utf8_general_ci;
grant all privileges on test.* to 'dbuser'@'localhost' identified by 'passwd' with grant option;
flush privileges;
exit
```

Install MariaDB Client Library
------------------------------
* sudo apt-get install libmysqlclient-dev

Install mysqlpp
---------------
* sudo apt-get install git
* git clone https://github.com/wasppdotorg/mysqlpp.git
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
* mariadb-10.0.23-winx64.msi

