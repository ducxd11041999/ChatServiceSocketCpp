#include "user.h"
#include <iostream>
#include <cstring>
using namespace std;


// set get of username
char *user::getUsername()
{
    char *temp = new char [SIZE_U];
    strcpy(temp, this->username);
    return temp;
}

void user::setUsername(char uName[])
{
    strcpy(this->username, uName);
}

// set get of password
char *user::getPassword()
{
    char *temp = new char [SIZE_P];
    strcpy(temp, this->password);
    return temp;
}

void user::setPassword(char pWord[])
{
    strcpy(this->password, pWord);
}

void user::setState(char State[])
{
    strcpy(this->state, State);
}

char *user::getState()
{
    char *temp = new char [SIZE_U];
    strcpy(temp, this->state);
    return temp;
}

// Constructor
user::user()
{
	strcpy(username, "");
	strcpy(password, "");
	strcpy(state,"");
    roles = 0;
}
void user::setroles(int role)
{
    this->roles =role;
}
int user::getroles()
{
    return roles;
}
