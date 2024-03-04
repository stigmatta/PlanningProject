#pragma once
#include "header.h"
#include "PlannerDlg.h"
class CCreateTaskDialog
{
public:
	CCreateTaskDialog(void);
	static CPlannerDlg* plannerDlg;
public:
	~CCreateTaskDialog(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CCreateTaskDialog* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	HWND hSpinHours, hSpinMinutes, hSpinSeconds,hEditHours,hEditMinutes,hEditSeconds,hEditTaskname;
	static HWND hCreateTaskDialog;
};
