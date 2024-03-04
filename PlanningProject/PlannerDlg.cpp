#include <fstream>
using namespace std;

#include "PlannerDlg.h"
#include "CreateTaskDialog.h"
#include "EditTaskDialog.h"

CPlannerDlg* CPlannerDlg::ptr = NULL;

CPlannerDlg::CPlannerDlg(void)
{
	ptr = this;
}

CPlannerDlg::~CPlannerDlg(void)
{
}

void CPlannerDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

typedef struct _MYDATA {
	LPTSTR szText;
	LPTSTR szTime;
	CPlannerDlg* plannerDlg;
};

void SaveCompletedTasksToFile(LPTSTR szText, LPTSTR szTime)
{
	wofstream file("done_tasks.txt", ios::out | ios::trunc);
	if (!file.is_open()) {
		MessageBox(NULL, TEXT("Failed to open file for writing."), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	file << szText << L"," << szTime << L"\n";

	file.close();

	MessageBox(NULL, TEXT("Completed tasks saved to file successfully."), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
}

void CALLBACK TimerCompletionRoutine(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer)
{
	_MYDATA* mydata = (_MYDATA*)Context;
	TCHAR szMessage[256];
	int index = SendMessage(CPlannerDlg::ptr->hListInProcess, LB_FINDSTRINGEXACT, -1, (LPARAM)mydata->szText);
	if (index != LB_ERR)
	{
		wsprintf(szMessage, TEXT("Task Name: %s\nTime: %s"), mydata->szText, mydata->szTime);
		MessageBox(NULL, szMessage, TEXT("Timer"), MB_OK | MB_ICONINFORMATION);
		SendMessage(CPlannerDlg::ptr->hListInProcess, LB_DELETESTRING, index, 0);
		SendMessage(CPlannerDlg::ptr->hListTime, LB_DELETESTRING, index, 0);
		SendMessage(CPlannerDlg::ptr->hListDone, LB_ADDSTRING, 0, (LPARAM)mydata->szText);
	}

	SaveCompletedTasksToFile(mydata->szText, mydata->szTime);

	delete[] mydata->szText;
	delete[] mydata->szTime;
	delete mydata;
}





void CPlannerDlg::CreateTaskCallback(TCHAR* szTaskName, TCHAR* szTime) {
	_MYDATA* mydata = new _MYDATA;

	mydata->szText = new TCHAR[_tcslen(szTaskName) + 1];
	mydata->szTime = new TCHAR[_tcslen(szTime) + 1];
	_tcscpy_s(mydata->szText, _tcslen(szTaskName) + 1, szTaskName);
	_tcscpy_s(mydata->szTime, _tcslen(szTime) + 1, szTime);

	SendMessage(hListInProcess, LB_ADDSTRING, 0, (LPARAM)szTaskName);
	SendMessage(hListTime, LB_ADDSTRING, 0, (LPARAM)szTime);
	int hours, minutes, seconds;
	_stscanf_s(szTime, _T("%d:%d:%d"), &hours, &minutes, &seconds);

	SYSTEMTIME st;
	GetLocalTime(&st);
	st.wHour = hours;
	st.wMinute = minutes;
	st.wSecond = seconds;

	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	LocalFileTimeToFileTime(&ft, &ft);

	HANDLE hTaskTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (hTaskTimer == NULL) {
		MessageBox(hDialog, TEXT("Failed to create task timer."), TEXT("Error"), MB_OK | MB_ICONERROR);
		delete[] mydata->szText;
		delete[] mydata->szTime;
		delete mydata;
		return;
	}

	PTP_TIMER ptpTimer = CreateThreadpoolTimer(TimerCompletionRoutine, mydata, NULL);
	if (ptpTimer == NULL) {
		MessageBox(hDialog, TEXT("Failed to create thread pool timer."), TEXT("Error"), MB_OK | MB_ICONERROR);
		CloseHandle(hTaskTimer);
		delete[] mydata->szText;
		delete[] mydata->szTime;
		delete mydata;
		return;
	}

	SetThreadpoolTimer(ptpTimer, &ft, 0, 0);

	CloseHandle(hTaskTimer);
}

BOOL CPlannerDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hListInProcess = GetDlgItem(hwnd, IDC_LISTINPROCESS);
	hListTime = GetDlgItem(hwnd, IDC_LISTTIME);
	hListDone = GetDlgItem(hwnd, IDC_LISTDONE);
	hListUndone = GetDlgItem(hwnd, IDC_LISTUNDONE);
	hButtonCreate = GetDlgItem(hwnd, IDBUTTONCREATE);
	hButtonEdit = GetDlgItem(hwnd, IDBUTTONEDIT);
	hButtonDelete = GetDlgItem(hwnd, IDBUTTONDELETE);
	hDialog = hwnd;
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	return TRUE;
}

void CPlannerDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	static CCreateTaskDialog createDlg;
	static CEditTaskDialog editDlg;

	if (id == IDBUTTONCREATE)
	{
		if (CCreateTaskDialog::hCreateTaskDialog)
		{
			SetForegroundWindow(CCreateTaskDialog::hCreateTaskDialog);
			ShowWindow(CCreateTaskDialog::hCreateTaskDialog, SW_RESTORE);
			return;
		}
		else
		{
			CCreateTaskDialog::hCreateTaskDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)CCreateTaskDialog::DlgProc);
			CCreateTaskDialog::plannerDlg = ptr;
			ShowWindow(CCreateTaskDialog::hCreateTaskDialog, SW_RESTORE);
			return;
		}
	}

	else if (id == IDBUTTONEDIT) {
		int selectedIndex = SendMessage(hListInProcess, LB_GETCURSEL, 0, 0);
		if (selectedIndex != LB_ERR)
		{
			if (CEditTaskDialog::hEditTaskDialog)
			{
				SetForegroundWindow(CEditTaskDialog::hEditTaskDialog);
				ShowWindow(CEditTaskDialog::hEditTaskDialog, SW_RESTORE);
				return;
			}
			else
			{
				CEditTaskDialog::hEditTaskDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwnd, (DLGPROC)CEditTaskDialog::DlgProc);
				CEditTaskDialog::plannerDlg = ptr;
				ShowWindow(CEditTaskDialog::hEditTaskDialog, SW_RESTORE);
				return;
			}
		}
		else
		{
			MessageBox(hwnd, TEXT("Please select a task to edit."), TEXT("No Task Selected"), MB_OK | MB_ICONWARNING);
			return;
		}
	}
	else if (id == IDBUTTONDELETE) {
		int selectedIndex = SendMessage(hListInProcess, LB_GETCURSEL, 0, 0);
		if (selectedIndex != LB_ERR) {
			SendMessage(CPlannerDlg::ptr->hListInProcess, LB_DELETESTRING, selectedIndex, 0);
			SendMessage(CPlannerDlg::ptr->hListTime, LB_DELETESTRING, selectedIndex, 0);
		}
	}
}



BOOL CALLBACK CPlannerDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
};