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

//�û���¼
INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//�û�ע��
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//�û��޸���Ϣ
INT_PTR CALLBACK DialogProcModify(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
//��½�ɹ���Ľ���
INT_PTR CALLBACK DialogProcSelect(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);