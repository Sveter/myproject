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

	void readFile(); //��ȡ�ļ��������´ε�¼��
	void saveFile(); //�����ļ�
	//WcharToString(username),WcharToString(pwd1), WcharToString(email), WcharToString(phone)
	bool login(string username, string pwd1);  //ע���û�
	bool logon(string name, string password);  //�û���¼
	string FindName(string name);
	string FindPwd(string name);
	//�޸���Ϣ
	bool modifypwd(string name, string newpwd);
	bool destroy(string name, string passwd); //ɾ���û�

private:
	unordered_map<string, string> _map;
	string _name;
	string _password;
};