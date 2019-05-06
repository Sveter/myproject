#include "User.h"
#include <fstream>
#include <iostream>

using namespace std;

//���ļ�����ȡ�û���Ϣ
void User::readFile()
{
	fstream file;
	file.open("user", ios::in);
	if (file.bad())
	{
		return;
	}
	int size = 0;
	file >> size;
	if (!size)
	{
		return;
	}

	while (size--)
	{
		string name;
		string password;
		file >> name;
		file >> password;
		_map.insert(make_pair(name, password));
	}
	file.close();
	return;
}

//�����ļ��������û���������
void User::saveFile()
{
	ofstream file("user");
	if (file.bad())
	{
		return;
	}
	int size = _map.size();
	file << size << endl;
	for (auto e : _map)
	{
		file << e.first << " " << e.second << endl;
	}
	file.close();
}


//ע���û�
bool User::login(string name, string pwd)
{
	unordered_map<string, string>::iterator got = _map.find(name);
	if (got != _map.end())
	{
		return false;
	}
	_map.insert(make_pair(name, pwd));
	saveFile();
	return true;
}

//�û���¼
bool User::logon(string name, string password)
{
	unordered_map<string, string>::iterator got = _map.find(name);
	if (got == _map.end())
	{
		//��¼ʧ��
		return false;
	}
	else
	{
		if (password == got->second)
		{
			//��½�ɹ�
			return true;
		}
		else
		{
			//��¼ʧ��
			return false;
		}
	}
}

//ɾ��һ���û�
bool User::destroy(string name, string passwd)
{
	unordered_map<string, string>::const_iterator got = _map.find(name);
	if (got == _map.end())
	{
		return false;
	}
	if(passwd == got->second)
	{
		_map.erase(got);
	}
	saveFile();
	return true;
}

string User::FindName(string name)
{
	unordered_map<string, string>::iterator got = _map.find(name);
	return got->first;
}

string User::FindPwd(string name)
{
	unordered_map<string, string>::iterator got = _map.find(name);
	return got->second;
}


bool User::modifypwd(string name, string newpwd)
{
	unordered_map<string, string>::iterator got = _map.find(name);
	if (got != _map.end())
	{
		_map.erase(got);
	}
	else
	{
		return false;
	}
	_map.insert(make_pair(name, newpwd));
	saveFile();
	return true;
}