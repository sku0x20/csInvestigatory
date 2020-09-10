#ifndef MYSQLCONNECT_H
#define MYSQLCONNECT_H

#include <iostream>
#include <windows.h>
#include <string>
#include "userstruct.h"
#include "users.h"

class mysqlconnect {

  private:
    //dataTypes user;
    HANDLE wHnd;
    void setCurPos(int x, int y);
    void setColour(int ForgC);
    string connectionString;
  public:
    /** Default constructor */
    mysqlconnect();
    bool connectStats();//done
    bool addRecord(dataTypes inputUser, string &pass); //done
    bool deleteRecord(unsigned int sId);//done
    bool editRecordAdmin(unsigned int sId, unsigned int income);//done
    bool searchRecord(char a[]);
    unsigned int lastId();//done
    bool retriveAllUsers();//done
    void editAddress(unsigned int id, string &address);//done
    void editEmail(unsigned int id, string &email);//done
    void editPhoneNo(unsigned int id, string phoneNo);//done
    void editPass(unsigned int id, string &pass);//done

  protected:

};

#endif // MYSQLCONNECT_H
