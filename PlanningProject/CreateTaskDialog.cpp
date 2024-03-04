#include <fstream>
#include <iostream>
#include <string>
#include "CreateTaskDialog.h"
#include "PlannerDlg.h"
using namespace std;

CCreateTaskDialog* CCreateTaskDialog::ptr = NULL;
CPlannerDlg* CCreateTaskDialog::plannerDlg = NULL;
HWND CCreateTaskDialog::hCreateTaskDialog = NULL;

CCreateTaskDialog::CCreateTaskDialog(void)
{
    ptr = this;
}

CCreateTaskDialog::~CCreateTaskDialog(void)
{

}

void CCreateTaskDialog::Cls_OnClose(HWND hwnd)
{
    DestroyWindow(hwnd);
    hCreateTaskDialog = NULL;
}


BOOL CCreateTaskDialog::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    hEditTaskname = GetDlgItem(hwnd, IDC_EDITTASKNAME);
    hSpinHours = GetDlgItem(hwnd, IDC_SPIN1);
    hSpinMinutes = GetDlgItem(hwnd, IDC_SPIN2);
    hSpinSeconds = GetDlgItem(hwnd, IDC_SPIN3);
    hEditHours = GetDlgItem(hwnd, IDC_EDITHOURS);
    hEditMinutes = GetDlgItem(hwnd, IDC_EDITMINUTES);
    hEditSeconds = GetDlgItem(hwnd, IDC_EDITSECONDS);
    SendMessage(hSpinHours, UDM_SETRANGE32, 0, 23);
    SendMessage(hSpinMinutes, UDM_SETRANGE32, 0, 59);
    SendMessage(hSpinSeconds, UDM_SETRANGE32, 0, 59);
    return TRUE;
}


void CCreateTaskDialog::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
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

            if (plannerDlg != NULL) {
                plannerDlg->CreateTaskCallback(szTaskName, szTime);

                CloseWindow(hwnd);
            }

            SetWindowText(hEditTaskname, _T(""));
            SetWindowText(hEditHours, _T(""));
            SetWindowText(hEditMinutes, _T(""));
            SetWindowText(hEditSeconds, _T(""));
        }
        else {
            MessageBox(hwnd, _T("Please fill in all fields."), _T("Error"), MB_OK | MB_ICONERROR);
        }
    }
}





BOOL CALLBACK CCreateTaskDialog::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}