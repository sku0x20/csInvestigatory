#include <iostream>
#include <windows.h>
#include "userAppInput.h"

using namespace std;

int main() {

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof (cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 12;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    userAppInput user;

    if(!user.getUser()) {
        return 0;
    }
Start:
    user.updateDispley();
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    user.UpdateMenu();

    for (;;) {
        if(!user.Navigation()) {
            return 0;
        } else if(user.enter) {
            break;
        }

        else if (user.updated == true) {
            user.updated = false;
            user.UpdateMenu();
        }
        Sleep(50);
    }

    user.enter = false;
    switch(user.M_Index) {
    case 4:
        user.editPhoneNo();
        break;
    case 3:
        user.editEmail();
        break;
    case 2:
        user.editAddress();
        break;
    case 1:
        user.editPass();
        break;
    case 0:
        user.qrCode();
        break;

    }

    system("pause > nul");
    goto Start;
}



