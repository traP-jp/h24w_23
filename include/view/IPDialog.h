#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <windows.h>

#include <string>

#include "resource.h"

class IPDialog
{
public:
    static BOOL CALLBACK
    DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            case WM_INITDIALOG:
                return TRUE;

            case WM_COMMAND:
                switch (LOWORD(wParam))
                {
                    case IDOK:
                    {
                        HWND hEdit = GetDlgItem(hWnd, IDC_EDIT1);
                        int len = GetWindowTextLength(hEdit) + 1;
                        char *ipAddress = new char[len];
                        GetWindowText(hEdit, ipAddress, len);

                        m_ipaddr = ipAddress;

                        delete[] ipAddress;
                        EndDialog(hWnd, IDOK);
                        return TRUE;
                    }
                    case IDCANCEL:
                        EndDialog(hWnd, IDCANCEL);
                        return TRUE;

                    case IDC_RADIO2:
                        m_isPlayer1 = true;
                        return TRUE;

                    case IDC_RADIO3:
                        m_isPlayer1 = false;
                        return TRUE;
                }

                break;
            default:
                break;
        }
        return FALSE;
    }

    static std::string GetIPAddr()
    {
        return m_ipaddr;
    }

    static bool IsPlayer1()
    {
        return m_isPlayer1;
    }

private:
    static std::string m_ipaddr;
    static bool m_isPlayer1;
};

#endif  // IPDIALOG_H
