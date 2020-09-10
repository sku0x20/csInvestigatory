#ifndef USERAPPINPUT_H
#define USERAPPINPUT_H

#include "mysqlconnect.h"
#include <string>
#include <windows.h>
#include "BitBuffer.hpp"
#include "QrCode.hpp"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;


class userAppInput {

  private:
    HANDLE wHnd;
    mysqlconnect mysqlFunc;
    bool qrFirst;
    bool restFirst;
    bool connectStats; // Implemented
    bool validateUser(unsigned int &id, string &pass);//done
    void getUserDetails();//done
    unsigned int n;
    string getPass(bool show_asterisk = true);//done
    void setCurPos(int x, int y);//done
    void setColour(int ForgC);//done
    string getAddress();//done
    void printQr(const QrCode &qr);//done
    void changeFont();//done

  public:
    bool enter = false;
    int M_Index = 0;
    bool updated = false;
    /** Default constructor */
    userAppInput();
    bool getUser();//done
    void createScreen();//done
    bool Navigation();//done
    void UpdateMenu();//done
    void clearFrame();//done
    void editEmail();//done
    void editAddress();//done
    void editPhoneNo();//done
    void editPass();//done
    void qrCode();//done
    void updateDispley();//done

  protected:

};

#endif // USERAPPINPUT_H
