#include "user.h"
#include <iostream>

using namespace std;

user::user() {}

user::user(string uname, string pword): username(uname), password(pword)
{
    state.type = 0;
    state.name = "";
    cout << "username: " << username << endl;
    cout << "password: " << password << endl;
}

string user::getUsername()
{
    return user::username;
}

void user::setUsername(string username)
{
    user::username = username;
}

string user::getPassword()
{
    return user::password;
}

void user::setPassword(string pword)
{
    user::password = pword;
}

STATE user::getState()
{
    return user::state;
}

void user::setState(int type, string name)
{
    user::state.type = type;
    user::state.name = name;
}


int user::getRole()
{
    return user::role;
}


void user::setRole(int role)
{
    user::role = role;
}

