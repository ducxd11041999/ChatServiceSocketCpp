#ifndef USER_H
#define USER_H
#include <string>

using namespace std;

//Define STATE for the user
struct STATE
{
    int type; // 0 is empty, 1 is user, 2 is room
    string name; // this is room's or user's name
};

class user
{
public:
    user(); // empty constructor
    user(string uname, string pword); // constructor

    void setUsername(string username);// set username
    string getUsername(); // get username

    void setPassword(string pword); // set password
    string getPassword(); // get password

    void setState(int type, string name); // set state
    STATE getState(); // get state

    // suport find in map


    void setRole(int role); //set role
    int getRole(); //get role

    //support find in map
    bool operator < (const user&u) const{

    if(username < u.username) return true;
    return false;

    }

private:
    string username; // this is username,
    string password; // this is password,
    STATE state; // this is the user's state

    int role = 0; // this is to determine the user's role

};

#endif // USER_H

