#include "database.h"
#include <mysql.h>
#include <stdio.h>

#define DB_NAME "ggbond"
#define DB_HOST "127.0.0.1"
#define DB_PORT 3306
#define DB_USER "root"
#define DB_USER_PASSWORD "123234abc"

static bool connect_db(MYSQL& mysql);


/*通过用户名和密码从数据库获取user信息
  返回值： 成功true,失败false*/

bool fetch_user_info(userinfo &user)
{
	MYSQL mysql;
	MYSQL_RES* res; //查询结果集
	MYSQL_ROW row;  //记录结构体
	char sql[256];
	bool ret = false;

	//1.连接到数据库
	if (connect_db(mysql) == false)
	{
		return false;
	}

	//2.根据用户名和密码获取用户信息(id, level_id)
	snprintf(sql, 256, "select id, level_id from users where username = '%s' and password = md5('%s');", user.username.c_str(), user.password.c_str());  //c_str()将string转换为char数组
	ret = mysql_query(&mysql, sql); //成功返回0
	if (ret )
	{
		printf("数据库查询出错， %s 错误原因: %s\n", sql, mysql_error(&mysql));
		mysql_close(&mysql);   //先关闭数据库，不占用资源
		return false;
	}

	//3.获取结果
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) //没有查到记录
	{
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);    //atoi()可以将字符串转换为整形变量
	user.level_id = atoi(row[1]);
	printf("user_id: %d \nlevel_id: %d\n", user.id, user.level_id);  //打印ID
	Sleep(500);
	//4.返回结果

	//释放结果集
	mysql_free_result(res);

	//关闭数据库
	mysql_close(&mysql);

	return true;
}


bool connect_db(MYSQL& mysql)
{
	//1.初始化数据库句柄
	mysql_init(&mysql);

	//2.设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");  /*gbk为中文字符编码（包含繁体）*/

	//3.连接数据库
	if (mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWORD, DB_NAME, DB_PORT, NULL, 0) == NULL)
	{
		printf("数据库连接出错， 错误原因: %s\n", mysql_error(&mysql));
		return false;
	}
	printf("数据库连接成功\n", mysql_error(&mysql));
	Sleep(500);    //视觉效果，可取消
	return true;
}