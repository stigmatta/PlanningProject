#pragma once
#include "header.h"
#include "PlannerDlg.h"
class CEditTaskDialog
{
public:
	CEditTaskDialog(void);
	static CPlannerDlg* plannerDlg;
public:
	~CEditTaskDialog(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CEditTaskDialog* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	HWND hSpinHours, hSpinMinutes, hSpinSeconds, hEditHours, hEditMinutes, hEditSeconds, hEditTaskname;
	static HWND hEditTaskDialog;
};
