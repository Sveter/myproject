#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

using namespace std;

class User
{
public:
	User()
		:_name("")
		,_password("")
	{
		_map.insert(make_pair("admin", "123456"));
	}
	
	~User()
	{
		_map.clear();
	}

	void readFile(); //读取文件（便于下次登录）
	void saveFile(); //存入文件
	//WcharToString(username),WcharToString(pwd1), WcharToString(email), WcharToString(phone)
	bool login(string username, string pwd1);  //注册用户
	bool logon(string name, string password);  //用户登录
	string FindName(string name);
	string FindPwd(string name);
	//修改信息
	bool modifypwd(string name, string newpwd);
	bool destroy(string name, string passwd); //删除用户

private:
	unordered_map<string, string> _map;
	string _name;
	string _password;
};