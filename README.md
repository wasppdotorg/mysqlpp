
mysqlpp
=======

Build Environment
-----------------
* GCC 5.4 - Ubuntu 16.04 LTS
* Visual Studio 2015 - Windows 10

Install MariaDB
---------------
* sudo apt-key adv --recv-keys --keyserver hkp://keyserver.ubuntu.com:80 0xF1656F24C74CD1D8
* sudo nano /etc/apt/sources.list
```
# http://downloads.mariadb.org/mariadb/repositories/
deb [arch=amd64,i386] http://ftp.kaist.ac.kr/mariadb/repo/10.1/ubuntu xenial main
deb-src http://ftp.kaist.ac.kr/mariadb/repo/10.1/ubuntu xenial main
```

* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo /etc/init.d/mysql stop
* sudo /usr/bin/mysqld_safe --skip-grant-tables &
* mysql -u root
```
update mysql.user set plugin='mysql_native_password';
quit;
```

* sudo kill -9 $(pgrep mysql)
* sudo /etc/init.d/mysql start
* mysql -u root -p
```
create database test default character set utf8 default collate utf8_general_ci;
grant all privileges on test.* to 'dbuser'@'localhost' identified by 'passwd' with grant option;
flush privileges;
exit
```

Install MariaDB (Windows)
-------------------------
* mariadb-10.1.14-winx64.msi

Install MariaDB Client Library
------------------------------
* sudo apt-get install libmariadbclient-dev

Install mysqlpp
---------------
* sudo apt-get install git
* git clone https://github.com/wasppdotorg/mysqlpp.git
* cd mysqpp
* sudo apt-get install build-essential
* sudo apt-get install cmake
* mkdir cmake_build
* cd cmake_build
* cmake ..
* make

Run
---
* ./mysqlpp

Run (Windows)
-------------
* Run cmd.exe
```
cd path\to\mysqlpp
copy "C:\Program Files\Mariadb 10.1\lib\libmysql.dll" .\x64\Debug\
.\mysqlpp.exe
```

Memory Leak Check
-----------------
* sudo apt-get install valgrind
* valgrind ./mysqlpp
