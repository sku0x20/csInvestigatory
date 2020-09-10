#include "inputclass.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <conio.h>
#include "picosha2.h"
#include "userstruct.h"
#include "users.h"

using namespace std;

inputclass::inputclass() {
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    connectStats = mysqlFunc.connectStats();
    if(connectStats) {
        uploadConfigData();
    }
    if(connectStats && (mysqlFunc.lastId() != userFunc.lastId())) {
        setCurPos(4, 24);
        setColour(12);
        cout << "Local File is corrupted. Retrieving from Mysql ";
        retriveAndReplaceData();
    }
}

bool inputclass::configAdd(dataTypes user, fstream &file, char hashedPassword[]) {
    short n;
    file.seekg(0);
    file.seekg(sizeof(int));
    file.read((char *)&n, sizeof(short));
    if(n > 9) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Connect Internet Cannot store more than 10";
        setColour(15);
        return false;
    }
    file.seekp(0);
    file.seekp(sizeof(int) + (3 * sizeof(short)) + (n * (sizeof(user)+ 65)));
    file.write((char *)&user, sizeof(user));
    file.write(hashedPassword, 65);
    n = n + 1;
    file.seekp(0);
    file.seekp(sizeof(int));
    file.write((char *)&n, sizeof(short));
    file.seekp(0);
    return true;
}

bool inputclass::configEdit(unsigned int id, unsigned int income) {
    if(!userFunc.idExist(id)) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Id Does NOt exist";
        setColour(15);
        return false;
    }
    fstream file(".\\config", ios::binary | ios::in | ios::out);
    dataTypes user;
    short n;
    file.seekg(0);
    file.seekg(sizeof(int) + sizeof(short));
    file.read((char *)&n, sizeof(short));
    if(n > 9) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Connect Internet Cannot store more than 10" ;
        setColour(15);
        return false;
    }
    file.seekp(0);
    file.seekp(sizeof(int) + (3 * sizeof(short)) + (10 * sizeof(user)) + (n * 2 * sizeof(int)));
    file.write((char *)&id, sizeof(int));
    file.write((char *)&income, sizeof(int));
    n = n + 1;
    file.seekp(0);
    file.seekp(sizeof(int) + sizeof(short));
    file.write((char *)&n, sizeof(short));
    file.close();
    return true;
}

bool inputclass::configDelete(unsigned int id) {
    if(!userFunc.idExist(id)) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Id Does NOt exist";
        setColour(15);
        return false;
    }
    fstream file(".\\config", ios::binary | ios::in | ios::out);
    dataTypes user;
    short n;
    file.seekg(0);
    file.seekg(sizeof(int) + (2 * sizeof(short)));
    file.read((char *)&n, sizeof(short));
    if(n > 9) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Connect Internet Cannot store more than 10" ;
        setColour(15);
        return false;
    }
    file.seekp(0);
    file.seekp(sizeof(int) + (3 * sizeof(short)) + (10 * sizeof(user)) + (20 * sizeof(int)) + (n * sizeof(int)));
    file.write((char *)&id, sizeof(int));
    n = n + 1;
    file.seekp(0);
    file.seekp(sizeof(int) + (2 * sizeof(short)));
    file.write((char *)&n, sizeof(short));
    file.close();
    return true;
}

void inputclass::inputAdd() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    unsigned int cId;
    setColour(15);
    fstream file(".\\config", ios::binary | ios::in | ios::out);
    fstream passFile(".\\usersPass", ios::binary | ios::app);
    file.seekg(0);
    file.read((char *)&cId, sizeof(cId));
    cId = cId + 1;
    dataTypes user;
    user.id = cId;
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
    setCurPos(21, 19);
    cout << cId;
    setCurPos(21, 22);
    cin.getline(user.name, 31);
    setCurPos(21, 25);
    cin.getline(user.email, 51);
    setCurPos(21, 28);
    strcpy(user.address, getAddress().c_str());
    cin.ignore(256, '\n');
    setCurPos(21, 33);
    cin.getline(user.phoneNo, 11);
    while(strlen(user.phoneNo) < 10) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Phone No. less than 10 ";
        setColour(15);
        setCurPos(21, 33);
        cout << "           ";
        setCurPos(21, 33);
        cin.getline(user.phoneNo, 11);
    }
    setCurPos(21, 36);
    cin >> user.income;
    setCurPos(21, 39);
    string password = getPass();
    string hashedPassword;
    picosha2::hash256_hex_string(password, hashedPassword);
    char hashedChar[65];
    strcpy(hashedChar, hashedPassword.c_str());
    setCurPos(9, 46);
    setColour(6);
    cout << "Adding...";
    if(connectStats) {
        bool mysqlQueryStatus = mysqlFunc.addRecord(user, hashedPassword);
        if(!mysqlQueryStatus) {
            connectStats = false;
            if(configAdd(user, file, hashedChar)) {
                userFunc.addRecord(user);
                passFile.write((char *)&cId, sizeof(cId));
                passFile.write((char *)&hashedChar, sizeof(hashedChar));
                file.seekp(0);
                file.write((char *)&cId, sizeof(cId));
            }
        } else {
            userFunc.addRecord(user);
            passFile.write((char *)&cId, sizeof(cId));
            passFile.write((char *)&hashedChar, sizeof(hashedChar));
            file.seekp(0);
            file.write((char *)&cId, sizeof(cId));
        }
        file.close();
    } else {
        if(configAdd(user, file, hashedChar)) {
            userFunc.addRecord(user);
            passFile.write((char *)&cId, sizeof(cId));
            passFile.write((char *)&hashedChar, sizeof(hashedChar));
            file.seekp(0);
            file.write((char *)&cId, sizeof(cId));
            file.close();
        } else {
            file.close();
        }
    }
    passFile.close();
    setCurPos(9, 47);
    setColour(9);
    cout << "Added";
    return;
}

void inputclass::inputDelete() {
    setCurPos(9, 17);
    cout << "      Id : ";
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    setCurPos(21, 17);
    cin >> sId;
    if(!userFunc.getInfo(sId)) {
        return;
    }
    if(connectStats) {
        bool mysqlQueryStatus = mysqlFunc.deleteRecord(sId);
        if(mysqlQueryStatus && (!userFunc.idExist(sId))) {
            setCurPos(9, 45);
            setColour(12);
            cout << "Local File is corrupted. Retrieving from Mysql ";
            setColour(15);
            mysqlFunc.retriveAllUsers();
        } else if(!mysqlQueryStatus) {
            connectStats = false;
            if(configDelete(sId)) {
                userFunc.deleteRecord(sId);
            }
        } else {
            userFunc.deleteRecord(sId);
        }
    } else {
        if(configDelete(sId)) {
            userFunc.deleteRecord(sId);
        }
    }
}

void inputclass::inputSearch() {
    cout << "Enter name ";
    cin.ignore(256, '\n');
    cin.getline(sName, 31);
    userFunc.searchRecord(sName);
}

void inputclass::inputAdminEdit() {
    unsigned int income;
    setCurPos(9, 17);
    cout << "      Id : ";
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    setCurPos(21, 17);
    cin >> sId;
    if(!userFunc.getInfo(sId)) {
        return;
    }
    setCurPos(9, 38);
    cout << "  Income : ";
    setCurPos(21, 38);
    cin >> income;
    if(connectStats) {
        bool mysqlQueryStatus = mysqlFunc.editRecordAdmin(sId, income);
        if(mysqlQueryStatus && (!userFunc.idExist(sId))) {
            setCurPos(9, 45);
            setColour(12);
            cout << "Local File is corrupted. Retrieving from Mysql ";
            setColour(15);
            mysqlFunc.retriveAllUsers();
        } else if(!mysqlQueryStatus) {
            connectStats = false;
            if(configEdit(sId, income)) {
                userFunc.editRecordAdmin(sId, income);
            }
        } else {
            userFunc.editRecordAdmin(sId, income);
        }
    } else {
        if(configEdit(sId, income)) {
            userFunc.editRecordAdmin(sId, income);
        }
    }
}

void inputclass::createConfig() {
    fstream file(".\\config", ios::binary | ios::out);
    unsigned int cId = mysqlFunc.lastId();
    unsigned short addNo = 0;
    unsigned short editNo = 0;
    unsigned short deleteNo = 0;
    dataTypes user;
    string uselessDataAddNo = string(10*sizeof(user), '\0');
    string uselessDataPassword = string(10*65, '\0');
    string uselessDataEditNo = string(20*sizeof(cId), '\0');
    string uselessDataDeleteNo = string(10*sizeof(cId), '\0');
    file.write((char *)&cId, sizeof(cId));
    file.write((char *)&addNo, sizeof(addNo));
    file.write((char *)&editNo, sizeof(editNo));
    file.write((char *)&deleteNo, sizeof(deleteNo));
    file.write(uselessDataAddNo.c_str(), uselessDataAddNo.size());
    file.write(uselessDataPassword.c_str(), uselessDataPassword.size());
    file.write(uselessDataEditNo.c_str(), uselessDataEditNo.size());
    file.write(uselessDataDeleteNo.c_str(), uselessDataDeleteNo.size());
    file.close();
}

void inputclass::uploadConfigData() {
    if(!connectStats) {
        setCurPos(9, 45);
        setColour(12);
        cout << "Internet Not Available... ";
        setColour(15);
        return;
    }
    setCurPos(9, 20);
    setColour(6);
    cout << "Uploading Users";
    uploadConfigAdd();
    uploadConfigEdit();
    uploadConfigDelete();
    createConfig();
    setCurPos(9, 47);
    setColour(9);
    Sleep(500);
    cout << "Done Uploading Saved Users ";
}

void inputclass::uploadConfigAdd() {
    short n;
    dataTypes user;
    string hashedPass;
    hashedPass.resize(65);
    fstream file(".\\config", ios::binary | ios::in);
    file.seekg(0);
    file.seekg(sizeof(int));
    file.read((char *)&n, sizeof(short));
    file.seekg(0);
    file.seekg(sizeof(int) + (3 * sizeof(short)));
    for(int i = 0; i < n; i++) { // i =1; i<=n
        file.read((char *)&user, sizeof(user));
        file.read(&hashedPass[0], 65);
        if(!mysqlFunc.addRecord(user, hashedPass)) {
            connectStats = false;
            break;
        }
    }
    file.close();
}

void inputclass::uploadConfigEdit() {
    short n;
    dataTypes user;
    unsigned int id;
    unsigned int income;
    fstream file(".\\config", ios::binary | ios::in);
    file.seekg(0);
    file.seekg(sizeof(int) + sizeof(short));
    file.read((char *)&n, sizeof(short));
    file.seekg(0);
    file.seekg(sizeof(int) + (3 * sizeof(short)) + (10 * sizeof(user)));
    for(int i = 0; i < n; i++) { // i =1; i<=n
        file.read((char *)&id, sizeof(int));
        file.read((char *)&income, sizeof(int));
        if(!mysqlFunc.editRecordAdmin(id, income)) {
            connectStats = false;
            break;
        }
    }
    file.close();
}

void inputclass::uploadConfigDelete() {
    short n;
    dataTypes user;
    unsigned int id;
    fstream file(".\\config", ios::binary | ios::in);
    file.seekg(0);
    file.seekg(sizeof(int) + (2 * sizeof(short)));
    file.read((char *)&n, sizeof(short));
    file.seekg(0);
    file.seekg(sizeof(int) + (3 * sizeof(short)) + (10 * sizeof(user)) + (20 * sizeof(int)));
    for(int i = 0; i < n; i++) { // i =1; i<=n
        file.read((char *)&id, sizeof(int));
        if(!mysqlFunc.deleteRecord(id)) {
            connectStats = false;
            break;
        }
    }
    file.close();
}

void inputclass::retriveAndReplaceData() {
    if(!mysqlFunc.retriveAllUsers()) {
        connectStats = false;
        return;
    }
    createConfig();
}

string inputclass::getPass(bool show_asterisk) {
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

string inputclass::getAddress() {
    string add;
    char ch;
    int i = 29;
    while((ch = cin.get())!= ';') {
        if((ch == '\n') && (i < 33)) {
            setCurPos(21, i);
            i++;
        } else if((ch == '\n') && (i >= 33)) {
            setCurPos(9, 45);
            setColour(12);
            cout << "Only this much Space For Address Available Please Reenter ";
            setColour(15);
            add.clear();
            for(int k = 21; k < 69; k++) {
                for(int j = 28; j < 33; j++) {
                    setCurPos(k, j);
                    cout << ' ';
                }
            }
            setColour(15);
            setCurPos(21, 28);
            i = 29;
        }
        add += ch;
        if(add.size() > 250) {
            setCurPos(9, 45);
            setColour(12);
            cout << "Address Is too long Please Reenter";
            setColour(15);
            add.clear();
            for(int k = 21; k < 69; k++) {
                for(int j = 28; j < 33; j++) {
                    setCurPos(k, j);
                    cout << ' ';
                }
            }
        }
    }
    return add;
}

void inputclass::setCurPos(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(wHnd, pos);
}

void inputclass::setColour(int ForgC) {
    WORD wColor;

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(wHnd, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(wHnd, wColor);
    }
    return;
}


