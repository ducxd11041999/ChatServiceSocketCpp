#ifndef ROLE_DA_H
#define ROLE_DA_H

#include "MySQLAccess.h"

#include <string>


using namespace std;

class rolesDA
{
public:
    rolesDA();
    ~rolesDA();
    string getAllRoles();

private:
    MySQLAccess my_conn;
};

#endif
