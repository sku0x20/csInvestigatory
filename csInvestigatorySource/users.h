#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <vector>
#include <windows.h>
#include "userstruct.h"

using namespace std;

class users {
  private:
    dataTypes user;
    HANDLE wHnd;
    void setCurPos(int x, int y);
    void setColour(int ForgC);

  public:
    /** Default constructor */
    users();
    void addRecord(dataTypes inputUser);
    void deleteRecord(unsigned int sId);
    void editRecordAdmin(unsigned int sId, unsigned int income);
    void searchRecord(char a[]);
    unsigned int lastId();
    void writeAllUsers(const vector<unsigned int> &id, const vector<string> &name, const vector<string> &phoneNo,
                       const vector<string> &address, const vector<string> &email, const vector<unsigned int> &income,
                       const vector<string> &pass);
    bool idExist(unsigned int id);
    bool getInfo(unsigned int id);

  protected:

};

#endif // USERS_H
