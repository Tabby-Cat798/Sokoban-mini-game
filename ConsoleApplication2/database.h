#pragma once
#include <string>

using namespace std;

typedef struct _userinfo{
	int id;          //用户id
	string username; //用户名
	string password; //密码
	int level_id;    //关卡id
}userinfo;

bool fetch_user_info(userinfo &user);