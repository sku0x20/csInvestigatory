#include "userAppInput.h"
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include "picosha2.h"
#include "mysqlconnect.h"
#include <string>
#include <cstring>
#include <conio.h>
#include <windows.h>
#include "BitBuffer.hpp"
#include "QrCode.hpp"

using namespace std;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

userAppInput::userAppInput() {
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    n = 0;
    qrFirst = true;
    restFirst = true;
    connectStats = mysqlFunc.connectStats();
    COORD bufferSize = {106, 51};

    SMALL_RECT windowSize = {0, 0, 105, 50};

    SetConsoleTitle("User Application");

    SetConsoleScreenBufferSize(wHnd, bufferSize);

    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    if(!connectStats) {
        setCurPos(63,45);
        setColour(9);
        cout << "Editing functionality will be disabled";
    }
    setColour(7);
}

bool userAppInput::getUser() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    string design = string(106, '#');

    setCurPos(0,0);

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
    setColour(15);
    cout << "Designed And Developed By:- SKU (2018-19)";
    setColour(7);
    string a = "`                                               `";
    setCurPos(28, 22);
    cout <<  "```LOGIN`````````````````````````````````````````" ;
    setCurPos(28, 27);
    cout <<  "`\t      Id :                                  `" ;
    setCurPos(28, 31);
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
    setCurPos(28, 28);
    cout <<  a;
    setCurPos(28, 29);
    cout <<  a;
    setCurPos(28, 30);
    cout <<  a;
    setCurPos(28, 32);
    cout <<  a;
    setCurPos(28, 33);
    cout <<  a;
    setCurPos(28, 34);
    cout <<  a;
    setCurPos(28, 35);
    cout <<  a;
    setCurPos(43, 27);
    unsigned int id;
    string pass;
    cin >> id;
    setCurPos(43, 31);
    pass = getPass();
    if(!validateUser(id, pass)) {
        return false;
    }
    system("cls");
    createScreen();
    dataTypes user;
    fstream file(".\\users", ios::binary | ios::in);
    while(true) {
        file.read((char *)&user, sizeof(user));
        if( file.eof() ) break;
        if(id == user.id) {
            setCurPos(9, 19);
            cout << "      Id : " << user.id;
            setCurPos(9, 22);
            cout << "    Name : " << user.name;
            setCurPos(9, 25);
            cout << "   Email : " << user.email;
            setCurPos(9, 28);
            cout << " Address : ";
            int j = 29;
            for(int i = 0; user.address[i] != '\0'; i++) {
                if(user.address[i] == '\n') {
                    setCurPos(20, j);
                    j++;
                    continue;
                }
                cout << user.address[i];
            }
            setCurPos(9, 33);
            cout << "Phone No : " << user.phoneNo;
            setCurPos(9, 36);
            cout << "  Income : " << user.income;
            break;
        }
        n++;
    }
    file.close();
    return true;
}

bool userAppInput::validateUser(unsigned int &id, string &pass) {
    fstream passFile(".\\usersPass", ios::binary | ios::in);
    unsigned int rId;
    string hashedPassword;
    picosha2::hash256_hex_string(pass, hashedPassword);
    hashedPassword.push_back('\0');
    string hashedPassRead;
    hashedPassRead.resize(65);
    while(true) {
        passFile.read((char *)&rId, sizeof(int));
        passFile.read(&hashedPassRead[0], 65);
        if( passFile.eof() ) break;
        if((id == rId) && (hashedPassword == hashedPassRead)) {
            passFile.close();
            return true;
        }
    }
    passFile.close();
    return false;
}

void userAppInput::editAddress() {
    clearFrame();
    if(!connectStats) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Please connect Internet ";
        setColour(15);
        return;
    }
    dataTypes user;
    fstream file(".\\users", ios::binary | ios::in | ios::out);
    file.seekg(0);
    file.seekg(n*sizeof(user));
    file.read((char *)&user, sizeof(user));
    setCurPos(9, 19);
    cout << "Current Address :- ";
    int j = 19;
    for(int i = 0; user.address[i] != '\0'; i++) {
        if(user.address[i] == '\n') {
            setCurPos(27, j);
            j++;
            continue;
        }
        cout << user.address[i];
    }
    setCurPos(9, 25);
    cout << "    New Address :- ";
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    setCurPos(27, 25);
    strcpy(user.address, getAddress().c_str());
    setCurPos(9, 46);
    setColour(6);
    cout << "Changing...";
    file.seekp(0);
    file.seekp(n*sizeof(user));
    file.write((char *)&user, sizeof(user));
    file.close();
    string add(user.address);
    mysqlFunc.editAddress(user.id, add);
    setCurPos(9, 47);
    setColour(9);
    Sleep(500);
    cout << "Changed ";
    setColour(15);
}

void userAppInput::editEmail() {
    clearFrame();
    if(!connectStats) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Please connect Internet ";
        setColour(15);
        return;
    }
    dataTypes user;
    char newMail[51];
    fstream file(".\\users", ios::binary | ios::in | ios::out);
    file.seekg(n*sizeof(user));
    file.read((char *)&user, sizeof(user));
    setCurPos(9, 19);
    cout << "Current Email :- ";
    cout << user.email;
    setCurPos(9, 21);
    cout << "    New Email :- ";
    setCurPos(27, 21);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    if(restFirst) {
        cin.getline(newMail, 51);
        restFirst = false;
    }
    cin.getline(newMail, 51);
    setCurPos(9, 46);
    setColour(6);
    strcpy(user.email, newMail);
    cout << "Changing...";
    file.seekp(n*sizeof(user));
    file.write((char *)&user, sizeof(user));
    file.close();
    string email(user.email);
    mysqlFunc.editEmail(user.id, email);
    setCurPos(9, 47);
    setColour(9);
    Sleep(500);
    cout << "Changed ";
    setColour(15);
}

void userAppInput::editPhoneNo() {
    clearFrame();
    if(!connectStats) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Please connect Internet ";
        setColour(15);
        return;
    }
    dataTypes user;
    char newPh[51];
    fstream file(".\\users", ios::binary | ios::in | ios::out);
    file.seekg(n*sizeof(user));
    file.read((char *)&user, sizeof(user));
    setCurPos(9, 19);
    cout << "Current Phone NO. :- ";
    setCurPos(9, 21);
    cout << "     New Phone NO :- ";
    setCurPos(30, 19);
    cout << user.phoneNo;
    setCurPos(30, 21);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    if(restFirst) {
        cin.getline(newPh, 51);
        restFirst = false;
    }
    cin.getline(newPh, 51);
    while(strlen(newPh) < 10) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Phone No. less than 10 ";
        setColour(15);
        setCurPos(30, 21);
        cout << "           ";
        setCurPos(30, 21);
        cin.getline(newPh, 11);
    }
    setCurPos(9, 46);
    setColour(6);
    cout << "Changing...";
    strcpy(user.phoneNo, newPh);
    file.seekp(n*sizeof(user));
    file.write((char *)&user, sizeof(user));
    file.close();
    string ph(user.phoneNo);
    mysqlFunc.editPhoneNo(user.id, ph);
    setCurPos(9, 47);
    setColour(9);
    Sleep(500);
    cout << "Changed ";
    setColour(15);
}

void userAppInput::editPass() {
    clearFrame();
    if(!connectStats) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Please connect Internet ";
        setColour(15);
        return;
    }
    fstream file(".\\usersPass", ios::binary | ios::out | ios::in);
    file.seekg(n * (sizeof(int)+ 65));
    unsigned int id;
    file.read((char *)&id, sizeof(int));
    setCurPos(9, 21);
    cout << "New Pass :- ";
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    setCurPos(27, 21);
    string password = getPass();
    string hashedPassword;
    picosha2::hash256_hex_string(password, hashedPassword);
    setCurPos(9, 46);
    setColour(6);
    cout << "Changing...";
    file.seekp((n * (sizeof(int) + 65)) + sizeof(int));
    file.write(hashedPassword.c_str(), 65);
    file.close();
    mysqlFunc.editPass(id, hashedPassword);
    setCurPos(9, 47);
    setColour(9);
    Sleep(500);
    cout << "Changed ";
    setColour(15);
}

string userAppInput::getPass(bool show_asterisk) {
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

void userAppInput::setCurPos(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(wHnd, pos);
}

void userAppInput::setColour(int ForgC) {
    WORD wColor;

    CONSOLE_SCREEN_BUFFER_INFO csbi;


    if (GetConsoleScreenBufferInfo(wHnd, &csbi)) {

        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(wHnd, wColor);
    }
    return;
}

void userAppInput::clearFrame() {
    string a = string(69, ' ');
    for(int i = 16; i < 49; i++) {
        setCurPos(1, i);
        cout << a;
    }
}

void userAppInput::createScreen() {
    COORD bufferSize = {106, 51};

    SMALL_RECT windowSize = {0, 0, 105, 50};

    SetConsoleTitle("User Application");

    SetConsoleScreenBufferSize(wHnd, bufferSize);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof (cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = 12;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(wHnd, FALSE, &cfi);

    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

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
    setColour(15);
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
    //setCurPos(0,0);
    //setCurPos(5,5);
}

bool userAppInput::Navigation() {


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

void userAppInput::UpdateMenu() {
    setCurPos(75, 22);
    if (M_Index == 4) {
        setColour(10);
        cout << (char)175 << " Edit Phone No ";
    } else {
        setColour(15);
        cout << " Edit Phone No " ;
    }


    setCurPos(75, 25);
    if (M_Index == 3) {
        setColour(10);
        cout << (char)175 << " Edit Email " ;
    } else {
        setColour(15);
        cout << " Edit Email " ;
    }


    setCurPos(75, 28);
    if (M_Index == 2) {
        setColour(10);
        cout << (char)175 << " Edit Address ";
    } else {
        setColour(15);
        cout << " Edit Address ";
    }


    setCurPos(75, 31);
    if (M_Index == 1) {
        setColour(10);
        cout << (char)175 << " Edit Pass " ;
    } else {
        setColour(15);
        cout << " Edit Pass ";
    }

    setCurPos(75, 34);
    if (M_Index == 0) {
        setColour(10);
        cout << (char)175  << " Qr Code ";
    } else {
        setColour(15);
        cout << " Qr Code ";
    }

    setColour(15);
}

void userAppInput::updateDispley() {
    clearFrame();
    dataTypes user;
    fstream file(".\\users", ios::binary | ios::in | ios::out);
    file.seekg(n*sizeof(user));
    file.read((char *)&user, sizeof(user));
    setCurPos(9, 19);
    cout << "      Id : " << user.id;
    setCurPos(9, 22);
    cout << "    Name : " << user.name;
    setCurPos(9, 25);
    cout << "   Email : " << user.email;
    setCurPos(9, 28);
    cout << " Address : ";
    int j = 29;
    for(int i = 0; user.address[i] != '\0'; i++) {
        if(user.address[i] == '\n') {
            setCurPos(20, j);
            j++;
            continue;
        }
        cout << user.address[i];
    }
    setCurPos(9, 33);
    cout << "Phone No : " << user.phoneNo;
    setCurPos(9, 36);
    cout << "  Income : " << user.income;
}

string userAppInput::getAddress() {
    string add;
    char ch;
    int i = 26;
    if(restFirst) {
        cin.get();
        restFirst = false;
    }
    while((ch = cin.get())!= ';') {
        if((ch == '\n') && (i < 30)) {
            setCurPos(27, i);
            i++;
        } else if((ch == '\n') && (i >= 30)) {
            setCurPos(9, 45);
            setColour(12);
            cout << "Only this much Space For Address Available Please Reenter ";
            setColour(15);
            add.clear();
            for(int k = 27; k < 69; k++) {
                for(int j = 25; j < 30; j++) {
                    setCurPos(k, j);
                    cout << ' ';
                }
            }
            setColour(15);
            setCurPos(27, 25);
            i = 26;
        }
        add += ch;
        if(add.size() > 250) {
            setCurPos(9, 45);
            setColour(12);
            cout << "Address Is too long Please Reenter";
            setColour(15);
            add.clear();
            for(int k = 27; k < 69; k++) {
                for(int j = 25; j < 30; j++) {
                    setCurPos(k, j);
                    cout << ' ';
                }
            }
        }
    }
    return add;
}

void userAppInput::qrCode() {
    if(qrFirst) {
        qrFirst = false;
        return;
    }
    fstream userF(".\\users", ios::binary | ios::in);
    dataTypes user;
    userF.seekg(n*sizeof(user));
    userF.read((char *)&user, sizeof(user));
    string qrName = "qrId";
    qrName += std::to_string(user.id);
    qrName += ".svg";
    fstream file(qrName, ios::binary | ios::out);
    string text = std::to_string(user.id);
    text += '\n';
    text += user.name;
    text += '\n';
    text += user.phoneNo;
    text += '\n';
    text += user.email;
    text += '\n';
    text += user.address;
    text += '\n';
    text += std::to_string(user.income);
    const QrCode::Ecc errCorLvl = QrCode::Ecc::MEDIUM;


    const QrCode qr = QrCode::encodeText(text.c_str(), errCorLvl);
    printQr(qr);
    string stringText= qr.toSvgString(4);

    file.write(stringText.c_str(), stringText.size());
    string openFile = ".\\";
    openFile += qrName;
    file.close();
    userF.close();
    ShellExecute(nullptr, "open",openFile.c_str(),"","",SW_SHOW );
}

void userAppInput::printQr(const QrCode &qr) {
    int border = 0;
    string t = string(2, (char)124);
    int i;
    int j;
    int y;
    int x;
    for (y = -border, j = 17; y < qr.getSize() + border; y++,j++) {
        for (x = -border, i = 3; x < qr.getSize() + border; x++) {
            if(i > 69) {
                break;
            }
            setCurPos(i,j);
            std::cout << (qr.getModule(x, y) ? t : "  ");
            i = i + 2;
        }
        if(j > 47) {
            break;
        }
        //j++;
    }
}

void userAppInput::changeFont() {
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
