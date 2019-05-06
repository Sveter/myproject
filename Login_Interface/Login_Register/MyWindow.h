#pragma once
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include "resource.h"
#include "User.h"
#include "Qualification.h"
using namespace std;

//用户登录
INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//用户注册
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//用户修改信息
INT_PTR CALLBACK DialogProcModify(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//登陆成功后的界面
INT_PTR CALLBACK DialogProcSelect(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);