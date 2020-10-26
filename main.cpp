#include <iostream>
using namespace std;
#include "connection.h"
#include "connectionPool.h"
void test_pool(void){
	connectionPool* cp = connectionPool::getConnectionPool();
	for (int i = 0; i < 250; ++i)
	{
		char sql[1024] = { 0 };
		sprintf_s(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
		shared_ptr<connection> sp = cp->getConnection();
		sp->update(sql);
	}
}
void test_single(void) {
	connection conn;
	conn.connect("127.0.0.1", 3306, "root", "211901", "chat");
	char sql[1024] = { 0 };
	sprintf_s(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
	conn.connect("127.0.0.1", 3306, "root", "211901", "chat");
	conn.update(sql);
}
int main()
{
#if 1
	clock_t begin = clock();

	thread t1(test_pool);
	thread t2(test_pool);
	thread t3(test_pool);
	thread t4(test_pool);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	clock_t end = clock();
	cout << (end - begin) << "ms" << endl;
#endif

#if 0
	for (int i = 0; i < 10; ++i)
	{
		//connection conn;
		//char sql[1024] = { 0 };
		//sprintf_s(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
		//	"zhang san", 20, "male");
		//conn.connect("127.0.0.1", 3306, "root", "211901", "chat");
		//conn.update(sql);
		connectionPool* cp = connectionPool::getConnectionPool();
		shared_ptr<connection> sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf_s(sql, "insert into user(name,age,sex) values('%s',%d,'%s')","zhang san", 20, "male");
		sp->update(sql);
	}
#endif

	return 0;
}
