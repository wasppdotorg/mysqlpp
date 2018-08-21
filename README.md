
mysqlpp
=======

Build Environment
-----------------
* GCC 7.3 - Ubuntu 18.04 LTS
* Visual Studio 2017 - Windows 10

Install MariaDB
---------------
* sudo apt update
* sudo apt upgrade
* sudo apt install mariadb-server
* sudo /etc/init.d/mysql restart

* mysql -u root -p
```
create database test default character set utf8mb4 default collate utf8mb4_general_ci;
grant all privileges on test.* to 'dbuser'@'localhost' identified by 'passwd' with grant option;
flush privileges;
exit
```

Install MariaDB (Windows)
-------------------------
* mariadb-10.3.9-winx64.msi

Install MariaDB Client Library
------------------------------
* sudo apt install libmariadbclient-dev-compat

Install mysqlpp
---------------
* sudo apt install git
* git clone https://github.com/wasppdotorg/mysqlpp.git
* cd mysqpp
* sudo apt install build-essential
* sudo apt install cmake
* mkdir cmake_build
* cd cmake_build
* cmake ..
* make

Run
---
* ./mysqlpp

Run (Windows)
-------------
* cd path\to\mysqlpp
* copy "C:\Program Files\Mariadb 10.3\lib\libmysql.dll" .\x64\Debug\
* .\mysqlpp.exe

Memory Leak Check
-----------------
* sudo apt install valgrind
* valgrind ./mysqlpp
