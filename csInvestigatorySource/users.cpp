#include "users.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include "userstruct.h"

using namespace std;

users::users() {
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
}

void users::addRecord(dataTypes inputUser) {
    user = inputUser;
    fstream file(".\\users", ios::binary | ios::app);
    if(!file.is_open()) {
        cout << "Error" << endl;
    }
    file.write((char *)&user, sizeof(user));
    file.close();
}

void users::searchRecord(char a[]) {
    char sName[31];
    strcpy(sName, a);
    fstream file(".\\users", ios::binary | ios::in);

    while(true) {
        file.read((char *)&user, sizeof(user));
        if( file.eof() ) break;
        if(strcmp(sName, user.name) == 0) {
            cout << "id is " << user.id;
            cout << "name is " << user.name;
            cout << "phoneNo is " << user.phoneNo;
            cout << "address is " << user.address;
            cout << "email is " << user.email;
            cout << "income is " << user.income;
        }
    }
    file.close();
}

void users::deleteRecord(unsigned int sId) {
    unsigned int passId;
    char hashedPass[65];
    fstream file(".\\users", ios::binary | ios::in);
    fstream usersPass(".\\usersPass", ios::binary | ios::in);
    ofstream o;
    ofstream newPass(".\\newPass", ios::binary | ios::out);
    o.open(".\\newTemp",ios::out | ios::binary);
    file.seekg(0);
    while(true) {
        file.read((char *)&user, sizeof(user));
        usersPass.read((char *)&passId, sizeof(int));
        usersPass.read((char *)&hashedPass, 65);
        if( file.eof() ) break;
        if(sId == user.id && sId == passId) {
            continue;
        }
        o.write((char *)&user, sizeof(user));
        newPass.write((char *)&passId, sizeof(int));
        newPass.write((char *)&hashedPass, 65);
    }
    setCurPos(9, 46);
    setColour(6);
    cout << "deleting...";
    file.close();
    o.close();
    usersPass.close();
    newPass.close();
    remove(".\\users");
    rename(".\\newTemp", ".\\users");
    remove(".\\usersPass");
    rename(".\\newPass", ".\\usersPass");
    setCurPos(9, 47);
    setColour(9);
    Sleep(500);
    cout << "Deleted";
    setColour(15);
}

void users::editRecordAdmin(unsigned int sId, unsigned int income) {
    unsigned long pos;
    fstream file(".\\users", ios::binary | ios::in | ios::out);
    file.seekg(0);
    while(true) {
        file.read((char *)&user, sizeof(user));
        if( file.eof() ) break;
        if(sId == user.id) {
            pos = file.tellg() - sizeof(user);
            break;
        }
    }
    user.income = income;
    setCurPos(9, 46);
    setColour(6);
    cout << "Making changes";
    file.seekp(pos);
    file.write((char *)&user, sizeof(user));
    setCurPos(9, 47);
    setColour(9);
    Sleep(500);
    cout << "successfully changed";
    setColour(15);
    file.close();

}

unsigned int users::lastId() {
    fstream file(".\\users", ios::binary | ios::in);
    file.seekg(0);
    file.seekg(-sizeof(user), ios::end);
    unsigned int id;
    file.read((char *)&id, sizeof(int));
    file.close();
    return id;
}

void users::writeAllUsers(const vector<unsigned int> &id, const vector<string> &name, const vector<string> &phoneNo,
                          const vector<string> &address, const vector<string> &email, const vector<unsigned int> &income,
                          const vector<string> &pass) {
    fstream file(".\\retrievedUsers", ios::binary | ios::app);
    fstream passFile(".\\retrievedPass", ios::binary | ios::app);
    for(int i = 0; i < id.size(); i++) {
        user.id = id[i];
        user.income = income[i];
        strcpy(user.name, name[i].c_str());
        strcpy(user.phoneNo, phoneNo[i].c_str());
        strcpy(user.address, address[i].c_str());
        strcpy(user.email, email[i].c_str());
        file.write((char *)&user, sizeof(user));
        passFile.write((char *)&id[i], sizeof(int));
        passFile.write(pass[i].c_str(), 65);
    }
    file.close();
    passFile.close();
}

bool users::getInfo(unsigned int id) {
    bool flag = idExist(id);
    if(!flag) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Id Doesn't Exist";
        setColour(15);
        return flag;
    }
    fstream file(".\\users", ios::binary | ios::in );
    file.seekg(0);
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
    }
    file.close();
    return flag;
}

bool users::idExist(unsigned int id) {
    bool flag = false;
    fstream file(".\\users", ios::binary | ios::in );
    file.seekg(0);
    while(true) {
        file.read((char *)&user, sizeof(user));
        if( file.eof() ) break;
        if(id == user.id) {
            flag = true;
        }
    }
    file.close();
    return flag;
}

void users::setCurPos(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(wHnd, pos);
}

void users::setColour(int ForgC) {
    WORD wColor;

    CONSOLE_SCREEN_BUFFER_INFO csbi;


    if (GetConsoleScreenBufferInfo(wHnd, &csbi)) {

        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(wHnd, wColor);
    }
    return;
}
