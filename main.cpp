#include <iostream>
using namespace std;
#include "connection.h"
#include "connectionPool.h"
void test(void){
	connectionPool* cp = connectionPool::getConnectionPool();
	for (int i = 0; i < 250; ++i)
	{
		char sql[1024] = { 0 };
		sprintf_s(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
		shared_ptr<connection> sp = cp->getConnection();
		sp->update(sql);
		/*connection conn;
		char sql[1024] = { 0 };
		sprintf_s(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
			"zhang san", 20, "male");
		conn.connect("127.0.0.1", 3306, "root", "211901", "chat");
		conn.update(sql);*/
	}
}
int main()
{
#if 1
	int x = 10;

	auto add_x = [x](int a) { x *= 2; return a + x; };  // 复制捕捉x

	cout << add_x(10) << endl; // 输出 30


	connection conn;
	conn.connect("127.0.0.1", 3306, "root", "211901", "chat");

	/*connection conn;
	char sql[1024] = { 0 };
	sprintf_s(sql, "insert into user(name,age,sex) values('%s',%d,'%s')","zhang san", 20, "male");
	conn.connect("127.0.0.1", 3306, "root", "211901", "chat");
	conn.update(sql);*/

	clock_t begin = clock();

	thread t1(test);
	thread t2(test);
	thread t3(test);
	thread t4(test);

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
