#include "connection.h"
connection::connection()
{
	_alivetime = clock();
	mysql_init(&_conn);
}

connection::~connection()
{
	mysql_close(&_conn);
}
bool connection::connect(string ip, unsigned int port,string username, string password, string dbname) {
	MYSQL* p = mysql_real_connect(&_conn, ip.c_str(), username.c_str(), password.c_str(),dbname.c_str() , port, NULL, 0);
	return p != nullptr;
}
bool connection::update(string sql) {
	if (mysql_query(&_conn, sql.c_str())!=0)
	{
		printf("%s failed! : %s\n", sql,mysql_error);
		return false;
	}
	return true;
}
// ²éÑ¯²Ù×÷ select
MYSQL_RES* connection::query(string sql) {
	if(mysql_query(&_conn, sql.c_str()))
	{
		printf("%s failed! : %s\n" ,sql, mysql_error);
		return nullptr;
	}
	return mysql_store_result(&_conn);
}