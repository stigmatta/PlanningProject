#include <fstream>
#include <iostream>
#include <string>
#include "EditTaskDialog.h"
#include "PlannerDlg.h"
using namespace std;

CEditTaskDialog* CEditTaskDialog::ptr = NULL;
CPlannerDlg* CEditTaskDialog::plannerDlg = NULL;
HWND CEditTaskDialog::hEditTaskDialog = NULL;

CEditTaskDialog::CEditTaskDialog(void)
{
    ptr = this;
}

CEditTaskDialog::~CEditTaskDialog(void)
{

}

void CEditTaskDialog::Cls_OnClose(HWND hwnd)
{
    DestroyWindow(hwnd);
    hEditTaskDialog = NULL;
}


BOOL CEditTaskDialog::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    hEditTaskname = GetDlgItem(hwnd, IDC_EDITEDIT);
    hSpinHours = GetDlgItem(hwnd, IDC_SPIN11);
    hSpinMinutes = GetDlgItem(hwnd, IDC_SPIN22);
    hSpinSeconds = GetDlgItem(hwnd, IDC_SPIN33);
    hEditHours = GetDlgItem(hwnd, IDC_EDITHOURS2);
    hEditMinutes = GetDlgItem(hwnd, IDC_EDITMINUTES2);
    hEditSeconds = GetDlgItem(hwnd, IDC_EDITSECONDS2);
    SendMessage(hSpinHours, UDM_SETRANGE32, 0, 23);
    SendMessage(hSpinMinutes, UDM_SETRANGE32, 0, 59);
    SendMessage(hSpinSeconds, UDM_SETRANGE32, 0, 59);
    return TRUE;
}


void CEditTaskDialog::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    TCHAR szTaskName[100];
    TCHAR bufHours[100], bufMinutes[10], bufSeconds[10];
    TCHAR szTime[20];
    int hours, minutes, seconds;

    if (id == IDOK) {
        if (GetWindowTextLength(hEditTaskname) && GetWindowTextLength(hEditHours) && GetWindowTextLength(hEditMinutes) && GetWindowTextLength(hEditSeconds))
        {
            GetWindowText(hEditTaskname, szTaskName, 100);
            GetWindowText(hEditHours, bufHours, 10);
            GetWindowText(hEditMinutes, bufMinutes, 10);
            GetWindowText(hEditSeconds, bufSeconds, 10);

            hours = _tstoi(bufHours);
            minutes = _tstoi(bufMinutes);
            seconds = _tstoi(bufSeconds);

            if (hours < 0 || hours > 23) {
                MessageBox(hwnd, _T("Invalid hours. Please enter a value between 0 and 23."), _T("Error"), MB_OK | MB_ICONERROR);
                return;
            }

            if (minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
                MessageBox(hwnd, _T("Invalid minutes or seconds. Please enter values between 0 and 59."), _T("Error"), MB_OK | MB_ICONERROR);
                return;
            }

            swprintf_s(szTime, _countof(szTime), _T("%02d:%02d:%02d"), hours, minutes, seconds);

            int selectedIndex = SendMessage(plannerDlg->hListInProcess, LB_GETCURSEL, 0, 0);
            if (selectedIndex != LB_ERR) {
                SendMessage(plannerDlg->hListInProcess, LB_DELETESTRING, selectedIndex, 0);
                SendMessage(plannerDlg->hListInProcess, LB_INSERTSTRING, selectedIndex, (LPARAM)szTaskName);
                SendMessage(plannerDlg->hListTime, LB_DELETESTRING, selectedIndex, 0);
                SendMessage(plannerDlg->hListTime, LB_INSERTSTRING, selectedIndex, (LPARAM)szTime);
            }

            SetWindowText(hEditTaskname, _T(""));
            SetWindowText(hEditHours, _T(""));
            SetWindowText(hEditMinutes, _T(""));
            SetWindowText(hEditSeconds, _T(""));

            CloseWindow(hwnd);
        }
        else {
            MessageBox(hwnd, _T("Please fill in all fields."), _T("Error"), MB_OK | MB_ICONERROR);
        }
    }
}





BOOL CALLBACK CEditTaskDialog::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}
