#pragma once
#include <Winsock2.h>
#include <mysql.h>
#include <cstdio>
#include <string>
#include <time.h>
using namespace std;
class connection
{
public:
	connection();
	~connection();
	bool connect(string ip, unsigned int port, string username, string password, string dbname);
	// 更新操作 insert、delete、update
	bool update(string sql);
	// 查询操作 select
	MYSQL_RES* query(string sql);
	void refreshAlivetime() {
		_alivetime = clock();
	};
	clock_t getAlivetime() {
		return clock()-_alivetime;
	};
private:
	MYSQL _conn;    //一个数据库结构体
	clock_t _alivetime;
};

