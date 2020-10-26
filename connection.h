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
	// ���²��� insert��delete��update
	bool update(string sql);
	// ��ѯ���� select
	MYSQL_RES* query(string sql);
	void refreshAlivetime() {
		_alivetime = clock();
	};
	clock_t getAlivetime() {
		return clock()-_alivetime;
	};
private:
	MYSQL _conn;    //һ�����ݿ�ṹ��
	clock_t _alivetime;
};

