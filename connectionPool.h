#pragma once
#include "connection.h"
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <memory>
#include <functional>
#include <atomic>
using namespace std;
class connectionPool
{
public:
	static connectionPool* getConnectionPool() {
		static connectionPool p;
		return &p;
	};
	~connectionPool();
	shared_ptr< connection> getConnection();
private:
	connectionPool();
	queue<connection*> connQue;
	string ip;
	string username;
	string dbname;
	unsigned int port;
	string password;
	int initSize;
	int maxSize;
	int maxIdleTime;
	int connectionTimeout;
	
	mutex queueMutex;
	atomic<int> connCount;
	condition_variable cv;
	bool loadConfigFile();
	// 运行在独立的线程中，专门负责生产新连接
	void produceConnectionTask();
	// 扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
	void scannerConnectionTask();
};

