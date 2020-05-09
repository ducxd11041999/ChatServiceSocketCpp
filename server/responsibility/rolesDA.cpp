#include "rolesDA.h"
#include "MySQLAccess.h"
#include "../Util/util.h"
#include "../include/mysql/jdbc.h"

#include <iostream>
#include <string>

using namespace std;

rolesDA::rolesDA() {}

rolesDA::~rolesDA() {}

string rolesDA::getAllRoles()
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::Statement *stmt;
    sql::ResultSet *res;

    string roles ="**\n";

    stmt = conn->createStatement();
    res = stmt->executeQuery("SELECT * FROM roles;");
    while (res->next())
    {
        roles += "*"+ to_string(res->getInt(1)) + ". " + res->getString(2)+ "\n";
    }
    roles +="**\n Defaut is number 2. USER!";
    delete res;
    delete stmt;
    delete conn;

    return roles;
}
