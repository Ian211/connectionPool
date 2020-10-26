#include "connectionPool.h"
connectionPool::connectionPool() {
	if (!loadConfigFile())
	{
		printf("load config failed!\n");
		return;
	}
	for (int i = 0; i < initSize; i++)
	{
		connection *conn=new connection;
		conn->connect(ip,port,username,password,dbname);
		conn->refreshAlivetime();
		connQue.push(conn);
		connCount++;
	}
	thread produce(bind(&connectionPool::produceConnectionTask,this));
	produce.detach();
	thread scan(bind(&connectionPool::scannerConnectionTask,this));
	scan.detach();
}
connectionPool::~connectionPool() {

}
bool connectionPool::loadConfigFile() {
	FILE* configFile;
	fopen_s(&configFile, "mysql.ini", "r");
	if (configFile==nullptr)
	{
		printf("open config file failed!\n");
		return false;
	}
	while (!feof(configFile))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, configFile);
		string str = line;
		int idx = str.find('=', 0);
		if (idx==-1)
		{
			continue;
		}
		int endidx = str.find('\n', idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - idx - 1);
		if (key == "ip")
		{
			ip = value;
		}
		else if (key == "port")
		{
			port = atoi(value.c_str());
		}
		else if (key == "username")
		{
			username = value;
		}
		else if (key == "password")
		{
			password = value;
		}
		else if (key == "dbname")
		{
			dbname = value;
		}
		else if (key == "initSize")
		{
			initSize = atoi(value.c_str());
		}
		else if (key == "maxSize")
		{
			maxSize = atoi(value.c_str());
		}
		else if (key == "maxIdleTime")
		{
			maxIdleTime = atoi(value.c_str());
		}
		else if (key == "connectionTimeOut")
		{
			connectionTimeout = atoi(value.c_str());
		}
	}
	return true;
}


shared_ptr< connection> connectionPool::getConnection() {
	unique_lock<mutex> lock(queueMutex);
	while (connQue.empty())
	{
		if (cv.wait_for(lock, chrono::milliseconds(connectionTimeout)) == cv_status::timeout) {
			printf("获取空闲连接超时了...获取连接失败!");
			return nullptr;
		}
	}
	shared_ptr<connection> conn(connQue.front(), [&](connection* x){
		unique_lock<mutex> lock(queueMutex);
		x->refreshAlivetime();
		connQue.push(x);
	});
	connQue.pop();
	cv.notify_all();
	return conn;
}

void connectionPool::produceConnectionTask() {
	while (true)
	{
		unique_lock<mutex> lock(queueMutex);
		while (!connQue.empty())
		{
			cv.wait(lock);
		}
		if (connCount < maxSize)
		{
			connection* conn = new connection;
			conn->connect(ip, port, username, password, dbname);
			conn->refreshAlivetime();
			connQue.push(conn);
			connCount++;
			cv.notify_all();
		}
		else
		{
			printf("以达到最大连接数!\n");
		}
	}
}

void connectionPool::scannerConnectionTask() {
	while (true)
	{
		this_thread::sleep_for(chrono::seconds(maxIdleTime));
		unique_lock<mutex> lock(queueMutex);
		while (connCount > initSize && connQue.front()->getAlivetime() > maxIdleTime*1000)
		{
			connection* conn = connQue.front();
			connQue.pop();
			connCount--;
			delete conn;
		}
	}
}