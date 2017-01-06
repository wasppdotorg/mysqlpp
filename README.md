
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

* sudo apt update
* sudo apt upgrade
* sudo apt install mariadb-server
* sudo nano /etc/mysql/conf.d/maria.cnf
```
[mysqld]
..
character-set-server  = utf8 
collation-server      = utf8_general_ci 
character_set_server   = utf8 
collation_server       = utf8_general_ci 
```

* sudo /etc/init.d/mysql restart

<!--
* sudo /etc/init.d/mysql stop
* sudo /usr/bin/mysqld_safe --skip-grant-tables &
* mysql -u root
```
update mysql.user set plugin='mysql_native_password';
quit;
```

* sudo kill -9 $(pgrep mysql)
* sudo /etc/init.d/mysql start
-->

* mysql -u root -p
```
create database test default character set utf8 default collate utf8_general_ci;
grant all privileges on test.* to 'dbuser'@'localhost' identified by 'passwd' with grant option;
flush privileges;
exit
```

Install MariaDB (Windows)
-------------------------
* mariadb-10.1.18-winx64.msi

Install MariaDB Client Library
------------------------------
* sudo apt install libmariadbclient-dev

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
* copy "C:\Program Files\Mariadb 10.1\lib\libmysql.dll" .\x64\Debug\
* .\mysqlpp.exe

Memory Leak Check
-----------------
* sudo apt install valgrind
* valgrind ./mysqlpp
