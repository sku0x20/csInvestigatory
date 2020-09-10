#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include "picosha2.h"
#include "users.h"
#include "inputclass.h"

using namespace std;

HANDLE wHnd;

void setCurPos(int x, int y);
void createScreen();
bool Navigation();
void UpdateMenu();
void SetColour(int ForgC);
void changeFont();

void clearFrame();
void addUserDesc();
void editUserDesc();
void deleteUserDesc();
void uploadDesc();
void retriveDesc();
string getPass(bool show_asterisk);
bool login();

const int addUser = 4;
const int editUser = 3;
const int deleteUser = 2;
const int uploadUsers = 1;
const int retriveUsers = 0;

const string password = "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918";

bool enter = false;

int M_Index = 0;
bool updated = false;

int main() {
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetColour(7);
    if(!login()) {
        return 0;
    }
    system("cls");
    createScreen();
    inputclass input;
Start:
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    enter = false;
    UpdateMenu();

    for (;;) {
        if(!Navigation()) {
            return 0;
        } else if(enter) {
            break;
        }

        else if (updated == true) {
            updated = false;
            UpdateMenu();
        }
        Sleep(50);
    }

    enter = false;
    clearFrame();
    switch(M_Index) {
    case 4:
        input.inputAdd();
        break;
    case 3:
        input.inputAdminEdit();
        break;
    case 2:
        input.inputDelete();
        break;

    case 1:
        input.uploadConfigData();
        break;

    case 0:
        input.retriveAndReplaceData();
        break;
    }

    system("pause > nul");
    goto Start;
}





void SetColour(int ForgC) {
    WORD wColor;

    CONSOLE_SCREEN_BUFFER_INFO csbi;


    if (GetConsoleScreenBufferInfo(wHnd, &csbi)) {

        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(wHnd, wColor);
    }
    return;
}

bool Navigation() {


    while(true) {
        if (GetAsyncKeyState(VK_UP)& 0x0001) {
            if (M_Index < 4) {
                M_Index++;
                updated = true;
                break;
            }
        } else if(GetAsyncKeyState(VK_DOWN)& 0x0001) {
            if (M_Index > 0)
                M_Index--;
            updated = true;
            break;
        } else if(GetAsyncKeyState(VK_ESCAPE)& 0x8000) {
            return false;
        } else if(GetAsyncKeyState(VK_RETURN)& 0x8000) {
            enter = true;
            break;
        } else if(GetAsyncKeyState(VK_TAB)& 0x0001) {
            changeFont();
            break;
        }
        Sleep(100);

    }
    return true;
}

void UpdateMenu() {

    clearFrame();
    setCurPos(75, 22);
    if (M_Index == addUser) {
        addUserDesc();
        SetColour(10);
        cout << (char)175 << " Add User ";
    } else {
        SetColour(15);
        cout << " Add User " ;
    }


    setCurPos(75, 25);
    if (M_Index == editUser) {
        editUserDesc();
        SetColour(10);
        cout << (char)175 << " Edit Record " ;
    } else {
        SetColour(15);
        cout << " Edit Record " ;
    }


    setCurPos(75, 28);
    if (M_Index == deleteUser) {
        deleteUserDesc();
        SetColour(10);
        cout << (char)175 << " Delete Record ";
    } else {
        SetColour(15);
        cout << " Delete Record ";
    }

    setCurPos(75, 31);
    if (M_Index == uploadUsers) {
        uploadDesc();
        SetColour(10);
        cout << (char)175 << " Upload Data " ;
    } else {
        SetColour(15);
        cout << " Upload Data ";
    }

    setCurPos(75, 34);
    if (M_Index == retriveUsers) {
        retriveDesc();
        SetColour(10);
        cout << (char)175  << " Retrieve & Replace ";
    } else {
        SetColour(15);
        cout << " Retrieve & Replace ";
    }
    SetColour(15);
}

void clearFrame() {
    string a = string(69, ' ');
    for(int i = 16; i < 49; i++) {
        setCurPos(1, i);
        cout << a;
    }
}

void addUserDesc() {
    setCurPos(9, 19);
    cout << "      Id : ";
    setCurPos(9, 22);
    cout << "    Name : ";
    setCurPos(9, 25);
    cout << "   Email : ";
    setCurPos(9, 28);
    cout << " Address : ";
    setCurPos(9, 33);
    cout << "Phone No : ";
    setCurPos(9, 36);
    cout << "  Income : ";
    setCurPos(9, 39);
    cout << "Password : ";
    setCurPos(75, 22);
}

void editUserDesc() {
    setCurPos(9, 17);
    cout << "      Id : ";
    setCurPos(75, 25);
}
void deleteUserDesc() {
    setCurPos(9, 17);
    cout << "      Id : ";
    setCurPos(75, 28);
}
void uploadDesc() {
    setCurPos(4, 24);
    cout << "Explicitly Upload the users Saved ";
    setCurPos(4, 26);
    cout << "in absence of Internet to Main Database.";
    setCurPos(75, 31);
}
void retriveDesc() {
    setCurPos(4, 24);
    cout << "Explicitly Retrieve Users from the Main Database";
    setCurPos(75, 34);
}

void createScreen() {
    COORD bufferSize = {106, 51};

    SMALL_RECT windowSize = {0, 0, 105, 50};

    SetConsoleTitle("Admin Application");

    SetConsoleScreenBufferSize(wHnd, bufferSize);

    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof (cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 12;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(wHnd, FALSE, &cfi);

    string design = string(106, '#');

    cout << design;

    for(int i = 1; i < 50; i++) {
        setCurPos(0, i);
        cout << "#";
    }

    for(int i = 1; i < 50; i++) {
        setCurPos(105, i);
        cout << "#";
    }

    setCurPos(0, 15);

    cout << design;

    for(int i = 15; i < 50; i++) {
        setCurPos(70, i);
        cout << "#";
    }
    setCurPos(0,49);
    cout << design;
    setCurPos(31, 50);
    SetColour(15);
    cout << "Designed And Developed By:- SKU (2018-19)";
    setCurPos(2,4);
    cout << " ______                             __        ____    ____                _      ______" ;
    setCurPos(2,5);
    cout << "|_   _ \\                           [  |      |_   \\  /   _|              / |_  .' ____ \\ " ;
    setCurPos(2,6);
    cout << "  | |_) |_ .--. ,--.  _ .--.  .---. | |--.     |   \\/   |  _ .--.  .--./`| |-' | (___ \\_| _   __ .--. ";
    setCurPos(2,7);
    cout << "  |  __'[ `/'`\\`'_\\ :[ `.-. |/ /'`\\]| .-. |    | |\\  /| | [ `.-. |/ /'`\\;| |    _.____`. [ \\ [  ( (`\\] ";
    setCurPos(2,8);
    cout << " _| |__) | |   // | |,| | | || \\__. | | | |   _| |_\\/_| |_ | | | |\\ \\._//| |,  | \\____) | \\ '/ / `'.'. ";
    setCurPos(2,9);
    cout << "|_______[___]  \\'-;__[___||__'.___.[___]|__] |_____||_____[___||__.',__` \\__/   \\______.[\\_:  / [\\__) )";
    setCurPos(2,10);
    cout << "                                                                 ( ( __))                \\__.'         ";
}

void setCurPos(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(wHnd, pos);
}

string getPass(bool show_asterisk = true) {
    const char BACKSPACE=8;
    const char RETURN=13;

    string password;
    unsigned char ch=0;

    while((ch=getch())!=RETURN) {
        if(ch==BACKSPACE) {
            if(password.length()!=0) {
                if(show_asterisk)
                    cout <<"\b \b";
                password.resize(password.length()-1);
            }
        } else if(ch==0 || ch==224) {
            getch();
            continue;
        } else {
            password+=ch;
            if(show_asterisk)
                cout <<'*';
        }
    }
    cout << endl;
    return password;
}

bool login() {
    COORD bufferSize = {106, 51};

    SMALL_RECT windowSize = {0, 0, 105, 50};

    SetConsoleTitle("Admin Application");

    SetConsoleScreenBufferSize(wHnd, bufferSize);

    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof (cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 12;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(wHnd, FALSE, &cfi);

    string design = string(106, '#');

    cout << design;

    for(int i = 1; i < 50; i++) {
        setCurPos(0, i);
        cout << "#";
    }

    for(int i = 1; i < 50; i++) {
        setCurPos(105, i);
        cout << "#";
    }

    setCurPos(0, 15);

    cout << design;
    setCurPos(0,49);
    cout << design;
    setCurPos(31, 50);
    SetColour(15);
    cout << "Designed And Developed By:- SKU (2018-19)";
    SetColour(7);
    string a = "`                                               `";
    setCurPos(28, 22);
    cout <<  "```LOGIN`````````````````````````````````````````" ;
    setCurPos(28, 28);
    cout <<  "`\tPassword :                                  `";
    setCurPos(28, 36);
    cout <<  "`````````````````````````````````````````````````" ;
    setCurPos(28, 23);
    cout <<  a;
    setCurPos(28, 24);
    cout <<  a;
    setCurPos(28, 25);
    cout <<  a;
    setCurPos(28, 26);
    cout <<  a;
    setCurPos(28, 27);
    cout <<  a;
    setCurPos(28, 29);
    cout <<  a;
    setCurPos(28, 30);
    cout <<  a;
    setCurPos(28, 31);
    cout <<  a;
    setCurPos(28, 32);
    cout <<  a;
    setCurPos(28, 33);
    cout <<  a;
    setCurPos(28, 34);
    cout <<  a;
    setCurPos(28, 35);
    cout <<  a;
    setCurPos(43, 28);
    string pass = getPass();
    string hashedPass;
    picosha2::hash256_hex_string(pass, hashedPass);

    if(password == hashedPass) {
        return true;
    } else {
        return false;
    }
}

void changeFont() {
    static bool terminal = true;
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof (cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 12;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    terminal?wcscpy(cfi.FaceName, L"Lucida Console"):wcscpy(cfi.FaceName, L"Terminal");
    terminal = !terminal;
    SetCurrentConsoleFontEx(wHnd, FALSE, &cfi);
}
