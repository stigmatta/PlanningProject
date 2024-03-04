#pragma once
#include "header.h"

class CPlannerDlg
{
public:
	CPlannerDlg(void);
public:
	~CPlannerDlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CPlannerDlg* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void CreateTaskCallback(TCHAR* szTaskName,TCHAR* szTime);
	HWND hListInProcess, hListTime, hListDone, hListUndone,hDialog,hButtonCreate,hButtonEdit,hButtonDelete;
};
