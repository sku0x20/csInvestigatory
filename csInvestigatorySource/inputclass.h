#ifndef INPUTCLASS_H
#define INPUTCLASS_H

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "userstruct.h"
#include "users.h"
#include "mysqlconnect.h"

class inputclass {
  private:
    char sName[30];
    unsigned int sId;
    int pos;
    users userFunc;
    mysqlconnect mysqlFunc;
    HANDLE wHnd;
    bool connectStats; // Implemented
    bool configAdd(dataTypes user, fstream &file, char hashedPassword[65]);//done
    bool configEdit(unsigned int id, unsigned int income);//done
    bool configDelete(unsigned int id);//done
    void uploadConfigAdd();//done
    void uploadConfigEdit();//done
    void uploadConfigDelete();//done
    void createConfig();//done
    string getPass(bool show_asterisk = true);//done
    void setCurPos(int x, int y);//done
    void setColour(int ForgC);//done
    string getAddress();//done


  public:
    /** Default constructor */
    inputclass();
    void inputAdd();//done
    void inputDelete();//done
    void inputAdminEdit();//done
    void inputSearch();//Dont Want to implement
    void uploadConfigData();//done
    void retriveAndReplaceData();//done


  protected:

};

#endif // INPUTCLASS_H
