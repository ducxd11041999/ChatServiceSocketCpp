#ifndef USER_DA_H
#define USER_DA_H

#include "MySQLAccess.h"
#include "../models/user.h"

#include <string>


using namespace std;

class usersDA
{
public:
    usersDA();
    ~usersDA();
    user *selectAllUsers();
    user *findUserWithName(string name);
    int countAllUsers();
    int countUserWithName(string name);
    void updateState(user usr);
    void clearState(user usr);
    void insertUser(user usr);
    void modifyUser(user usr);
    void deleteUser(user usr);
    bool validateLogin(string username, string password);
private:
    MySQLAccess my_conn;
};

#endif
