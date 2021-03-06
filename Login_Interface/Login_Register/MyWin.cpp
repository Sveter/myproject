#include "MyWindow.h"
using namespace std;

vector<string> onlineuser; //当前登录用户
HINSTANCE hInst; //窗口
User users; //用户
QuaFunction qua; //申请操作
Qualifiation qf; //景点申请信息

//此函数用于将string转换为wchar
WCHAR* StringToWchar(const string& s) 
{ 
	int length = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0); 
	WCHAR* w_string = new WCHAR[length+1]; 
	memset(w_string, 0, sizeof(WCHAR)*length); 
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, w_string, length); 
	return w_string; 
} 
//此函数用于将WCHAR转换为string 
std::string WcharToString(const WCHAR* s) 
{ 
	int length = WideCharToMultiByte(CP_ACP, 0, s, -1, NULL, 0, NULL, false); 
	char* str = new char[length+1];
	memset(str, 0, length); 
	WideCharToMultiByte(CP_ACP, 0, s, -1, str, length, NULL, false); 
	return std::string(str); 
}

//用户注册界面
INT_PTR CALLBACK DialogProcSignUp(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: 
	{ 
		switch (LOWORD(wParam)) 
		{
		case IDC_BUTTON4:
		{ 
			//获取定义一个User类的信息 
			wchar_t username[20]; 
			wchar_t pwd1[30], pwd2[30]; 
			GetDlgItemTextW(hdlg, IDC_EDIT3, username, sizeof username);
			GetDlgItemTextW(hdlg, IDC_EDIT4, pwd1, sizeof pwd1); 
			GetDlgItemTextW(hdlg, IDC_EDIT5, pwd2, sizeof pwd2);
			//密码需要输入两次，两次必须一致，但是这里没有判断是否为空 			
			if (wcscmp(pwd1, pwd2))
			{ 
				MessageBox(hdlg, _T("密码不一致！"),_T("提示"), MB_OK); 
				break;
			} 
			//调用注册用户的接口 
			if (users.login(WcharToString(username), WcharToString(pwd1)) == true)
			{ 
				if (MessageBox(hdlg, _T("确定注册用户？"), _T("提示"), MB_OKCANCEL) == IDOK)
				{ 
					EndDialog(hdlg, IDC_BUTTON4);
					//跳转回用户登录对话框 
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);
				}
			} 
			else 
			{ 
				MessageBox(hdlg, _T("注册失败，该用户已存在"), _T("提示"), MB_OKCANCEL);
			} 
		} 
		break; 
		case IDC_BUTTON5: 
		{
			//点击取消按钮，也是跳转到用户登录对话框 
			EndDialog(hdlg, IDC_BUTTON5); 
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn); 
		} 
		break;
		} 
	} 
	break; 
	case WM_SYSCOMMAND:
	{ 
		if (wParam == SC_CLOSE) 
		{
			//点击关闭窗口，也是跳转到用户登录对话框
			EndDialog(hdlg, SC_CLOSE);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);
		} 
	} 
	break; 
	} 
	return (INT_PTR)FALSE; 
}

//用户修改密码对话框处理函数
INT_PTR CALLBACK DialogProcModify(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	wchar_t username[20];
	wchar_t pwd1[40], pwd2[40];
	switch (msg) {
	/*case WM_INITDIALOG:
	{
		wcscpy_s(username, StringToWchar(users.GetName()));
		wcscpy_s(pwd1, StringToWchar(users.GetPwd()));

		memset(pwd2, 0, sizeof pwd2);

		SetDlgItemTextW(hdlg, IDC_EDIT8, username);
		SetDlgItemTextW(hdlg, IDC_EDIT9, pwd1);
	}
	break;*/
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		//确认修改按钮 
		case IDC_BUTTON6:
		{
			if (MessageBox(hdlg, _T("确认修改？"), _T("提示"), MB_OKCANCEL) == IDOK)
			{
				GetDlgItemTextW(hdlg, IDC_EDIT8, username, sizeof username);
				GetDlgItemTextW(hdlg, IDC_EDIT9, pwd1, sizeof pwd1);
				GetDlgItemTextW(hdlg, IDC_EDIT10, pwd2, sizeof pwd2);
				
				wchar_t* pwd = StringToWchar(users.FindPwd(WcharToString(username)));
				if (wcscmp(pwd1, pwd))
				{
					MessageBox(hdlg, _T("旧密码有误！"), _T("提示"), MB_OK);
					break;
				}
				else if (lstrlenW(pwd2) == 0)
				{
					MessageBox(hdlg, _T("新密码不能为空！"), _T("提示"), MB_OK);
					break;
				}
				if (users.modifypwd(WcharToString(username), WcharToString(pwd2)) )
				{
					EndDialog(hdlg, IDC_BUTTON6);
					//修改成功，返回登录界面
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);
				}
				else
				{
					MessageBox(hdlg, _T("修改失败！请重试"), _T("提示"), MB_OK);
				}
			}
		}
		break; 
		case IDC_BUTTON8:
		{
			if (MessageBox(hdlg, _T("确认删除？"), _T("提示"), MB_OKCANCEL) == IDOK)
			{
				GetDlgItemTextW(hdlg, IDC_EDIT8, username, sizeof username);
				GetDlgItemTextW(hdlg, IDC_EDIT9, pwd1, sizeof pwd1);

				wchar_t* pwd = StringToWchar(users.FindPwd(WcharToString(username)));
				if (wcscmp(pwd1, pwd))
				{
					MessageBox(hdlg, _T("密码不匹配！"), _T("提示"), MB_OK);
					break;
				}
				else if(users.destroy(WcharToString(username), WcharToString(pwd1)))
				{
					EndDialog(hdlg, IDC_BUTTON8);
					//删除成功，返回登录界面
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);
				}
				else
				{
					MessageBox(hdlg, _T("删除失败！请重试"), _T("提示"), MB_OK);
				}
			}
		}
		break;
		//取消按钮 
		case IDC_BUTTON7: 
		{ 
			EndDialog(hdlg, IDC_BUTTON7);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);
		}
		break;
		}
	}
	 break;
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_CLOSE)
		{
			EndDialog(hdlg, SC_CLOSE);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);
		}
	}
	break;
	}
	return (INT_PTR)FALSE;
}

//用户登录对话框处理函数
INT_PTR CALLBACK DialogProcSignIn(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: //登录按钮
		{
			wchar_t username[20];
			wchar_t pwd[30];
			GetDlgItemTextW(hdlg, IDC_EDIT1, username, sizeof username);
			GetDlgItemTextW(hdlg, IDC_EDIT2, pwd, sizeof pwd);
			string name = WcharToString(username);
			string passwd = WcharToString(pwd);
			onlineuser.push_back(name);
			if (users.logon(name, passwd))
			{
				EndDialog(hdlg, IDC_BUTTON1);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DialogProcSelect);
			}
			else
			{
				MessageBox(hdlg, _T("用户名或者密码错误！"), _T("用户登录"), MB_OK);
			}
		}
		break;
		case IDC_BUTTON2: //注册按钮
		{
			EndDialog(hdlg, IDC_BUTTON2);
			//跳转到用户注册对话框
			int actionSignUp = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), NULL, DialogProcSignUp);
		}
		break;
		case IDC_BUTTON3: //修改密码
		{
			EndDialog(hdlg, IDC_BUTTON3);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), NULL, DialogProcModify);
		}
		break;
		}
	}
	break;
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_CLOSE) 
		{ 
			//防止用户不小心点到关闭窗口 
			if (MessageBox(hdlg, _T("确定退出程序？"), _T("提示"), MB_OKCANCEL) == IDOK)
			{ 
				PostQuitMessage(0); 
			} 
		}
	}
	break;
	}
	return (INT_PTR)FALSE;
}
//景点项目申报
INT_PTR CALLBACK DialogProcQua(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		//确认提交
		case IDC_BUTTON13:
		{
			if (MessageBox(hdlg, _T("确定创建会议？"), _T("提示"), MB_OKCANCEL) == IDOK)
			{
				//项目名称
				wchar_t projectname[20] ,controller[20], place[20], size[20],content[1024];
				memset(projectname, 0, sizeof projectname);
				memset(controller, 0, sizeof controller);
				memset(place, 0, sizeof place);
				memset(size, 0, sizeof size);
				memset(content, 0, sizeof content);
				GetDlgItemTextW(hdlg, IDC_EDIT11, projectname, sizeof projectname);
				GetDlgItemTextW(hdlg, IDC_EDIT12, controller, sizeof controller);
				GetDlgItemTextW(hdlg, IDC_EDIT13, place, sizeof place);
				GetDlgItemTextW(hdlg, IDC_EDIT14, size, sizeof size);
				GetDlgItemTextW(hdlg, IDC_EDIT15, content, sizeof content);
				string pname = WcharToString(projectname);
				string ctl = WcharToString(controller);
				string pl = WcharToString(place);
				string sz = WcharToString(size);
				string ct = WcharToString(content);
				if (lstrlenW(projectname) == 0)
				{
					MessageBox(hdlg, _T("项目名称未输入！"), _T("提示"), MB_OK);
					break;
				}
				//地点
				if (lstrlenW(controller) == 0)
				{
					MessageBox(hdlg, _T("管理者未输入！"), _T("提示"), MB_OK);
					break;
				}
				//管理者
				if (lstrlenW(place) == 0)
				{
					MessageBox(hdlg, _T("地点未输入！"), _T("提示"), MB_OK);
					break;
				}
				//规模
				if (lstrlenW(size) == 0)
				{
					MessageBox(hdlg, _T("规模未输入！"), _T("提示"), MB_OK);
					break;
				}
				//申请内容
				if (lstrlenW(content) == 0)
				{
					MessageBox(hdlg, _T("申请内容未输入！"), _T("提示"), MB_OK);
					break;
				}
				//设置会议时间 (ok)
				SYSTEMTIME applyDate = { 0 }; 
				HWND datepicker1 = GetDlgItem(hdlg, IDC_DATETIMEPICKER1);
				SendMessage(datepicker1, NULL, 0, (LPARAM)&applyDate);
					
				char time[20]; 
				sprintf_s(time, "%04u/%02u/%02u", applyDate.wYear, applyDate.wMonth, applyDate.wDay);
				string applytime(time);
				//申请信息生成到文件中
				if (qua.Insert(pname, ctl, pl, sz, applytime, ct))
				{ 
					EndDialog(hdlg, SC_CLOSE);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DialogProcSelect);
				} 
				else 
				{
					MessageBox(hdlg, _T("申请失败！请重试"), _T("提示"), MB_OK);
				}
			}
		}
		break;
		//取消按钮
		case IDC_BUTTON14:
		{
			EndDialog(hdlg, IDC_BUTTON14);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DialogProcSelect);
		}
		break;
		}
	}
	break;
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_CLOSE)
		{
			EndDialog(hdlg, SC_CLOSE);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, DialogProcSelect);
		}
	}
	break;
	}
	return (INT_PTR)FALSE;
}

//登陆成功后进入选择界面
INT_PTR CALLBACK DialogProcSelect(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		HWND hCurrentUser = GetDlgItem(hdlg, IDC_STATIC5); 
		SetWindowTextW(hCurrentUser, StringToWchar(users.FindName(onlineuser[0])));
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON9:
		{
			EndDialog(hdlg, IDC_BUTTON9);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), NULL, DialogProcQua);
		}
		break;
		case IDC_BUTTON12:
			if (MessageBox(hdlg, _T("确定退出登录？"), _T("提示"), MB_OKCANCEL) == IDOK) 
			{ 
				onlineuser.pop_back();
				EndDialog(hdlg, IDC_BUTTON12); 
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn); 
			}
			break;
		}
	}
	break;
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_CLOSE)
		{
			onlineuser.pop_back();
			EndDialog(hdlg, SC_CLOSE);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);
		}
	}
	break;
	}
	return (INT_PTR)FALSE;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	//加载上次存入文件的用户
	users.readFile();
	qua.ReadFile();
	hInst = hInstance;
	//首先进入用户登录界面
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProcSignIn);

	return 0;
}