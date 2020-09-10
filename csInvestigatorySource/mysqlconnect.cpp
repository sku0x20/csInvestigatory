#include "mysqlconnect.h"
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>
#include <vector>
#include <fstream>

using namespace soci;
using namespace std;

mysqlconnect::mysqlconnect() {
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    connectionString = "host=hostAddress user=userName db=dbName password='password'";//edit according to your needs
    //"host=hostAddress user=userName db=dbName password='password'"
}

bool mysqlconnect::connectStats() {
    try {
        session sql("mysql", connectionString);
        int id;
        sql << "select max(id) from employees", into(id);
        return true;
    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
        return false;
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}

bool mysqlconnect::addRecord(dataTypes inputUser, string &pass) {
    try {
        session sql("mysql", connectionString);
        int unsigned id = inputUser.id;
        int unsigned income = inputUser.income;
        string name(inputUser.name);
        string phoneNo(inputUser.phoneNo);
        string address(inputUser.address);
        string email(inputUser.email);
        sql << "insert into employees(id, name, phoneNo, address, email, income) values(:id, :nm, :ph, :add, :em, :in)",
            use(id), use(name), use(phoneNo), use(address), use(email), use(income);
        sql << "insert into employeesPass(id, userPass) values (:id, :pass)", use(id), use(pass);
        return true;

    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error" << endl;
        setColour(15);
        return false;
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}

bool mysqlconnect::deleteRecord(unsigned int sId) {
    try {
        session sql("mysql", connectionString);
        short exist;
        sql << "select count(id) from employees where id = :id", into(exist), use(sId);
        if(exist) {
            sql << "delete from employees where id = :id", use(sId);
            sql << "delete from employeesPass where id = :id", use(sId);
        } else {
            setCurPos(9, 45);
            setColour(12);
            cout << "USer dosent exist";
            setColour(15);
        }
        return true;

    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
        return false;
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}

bool mysqlconnect::editRecordAdmin(unsigned int sId, unsigned int income) {
    try {
        session sql("mysql", connectionString);
        short exist;
        sql << "select count(id) from employees where id = :id", into(exist), use(sId);
        if(exist) {
            sql << "update employees set income = :in where id = :id", use(income),use(sId);
        } else {
            setCurPos(9, 45);
            setColour(12);
            cout << "USer dosent exist";
            setColour(15);
        }
        return true;

    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
        return false;
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}

unsigned int mysqlconnect::lastId() {
    try {
        session sql("mysql", connectionString);
        unsigned int id;
        sql << "select max(id) from employees", into(id);
        return id;
    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}

bool mysqlconnect::retriveAllUsers() {
    try {
        setCurPos(9, 20);
        setColour(6);
        cout << "Retrieving Users";
        session sql("mysql", connectionString);
        unsigned int mId;
        sql << "select count(id) from employees", into(mId);
        users usersObj;
        vector<unsigned int> id;
        vector<unsigned int> income;
        vector<string> name;
        vector<string> phoneNo;
        vector<string> address;
        vector<string> email;
        vector<string> pass;
        unsigned int m  = (mId%10 == 0)?((mId/10)-1):(mId/10);
        for(int i = 0; i <= m; i++) {
            setCurPos(9, 20);
            id.resize(100);
            income.resize(100);
            name.resize(100);
            phoneNo.resize(100);
            address.resize(100);
            email.resize(100);
            pass.resize(100);
            sql << "select id, name, phoneNo, address, email, income from employees limit 10 offset :n",
                into(id), into(name), into(phoneNo), into(address), into(email), into(income), use(i * 10);
            sql << "select userPass from employeesPass limit 10 offset :n",
                into(pass), use(i * 10);
            usersObj.writeAllUsers(id, name, phoneNo, address, email, income, pass);
            id.clear();
            income.clear();
            name.clear();
            phoneNo.clear();
            address.clear();
            email.clear();
        }
        remove(".\\users");
        rename(".\\retrievedUsers", ".\\users");
        remove(".\\usersPass");
        rename(".\\retrievedPass", ".\\usersPass");
        setCurPos(9, 47);
        setColour(9);
        Sleep(500);
        cout << "Done Retrieving users ";
        setColour(15);
        return true;
    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error" ;
        setColour(15);
        return false;
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}

void mysqlconnect::editAddress(unsigned int id, string &address) {
    try {
        session sql("mysql", connectionString);
        sql << "update employees set address = :add where id = :id", use(address),use(id);
    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
    }
}

void mysqlconnect::editEmail(unsigned int id, string &email) {
    try {
        session sql("mysql", connectionString);
        sql << "update employees set email = :em where id = :id", use(email),use(id);
    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
    }
}
void mysqlconnect::editPhoneNo(unsigned int id, string phoneNo) {
    try {
        session sql("mysql", connectionString);
        sql << "update employees set phoneNo = :ph where id = :id", use(phoneNo),use(id);
    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}
void mysqlconnect::editPass(unsigned int id, string &pass) {
    try {
        session sql("mysql", connectionString);
        sql << "update employeesPass set userPass = :pass where id = :id", use(pass),use(id);
    } catch (soci_error const &e) {
        setCurPos(9, 45);
        setColour(12);
        cerr << "Error: NO Internet OR Server Error";
        setColour(15);
        //cerr << "Error: " << e.get_error_message() << '\n';
    }
}

void mysqlconnect::setCurPos(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(wHnd, pos);
}

void mysqlconnect::setColour(int ForgC) {
    WORD wColor;

    CONSOLE_SCREEN_BUFFER_INFO csbi;


    if (GetConsoleScreenBufferInfo(wHnd, &csbi)) {

        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(wHnd, wColor);
    }
    return;
}



