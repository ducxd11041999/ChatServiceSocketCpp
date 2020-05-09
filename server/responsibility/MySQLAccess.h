#ifndef MYSQLACCESS_H
#define MYSQLACCESS_H


#include "../include/mysql/jdbc.h"


class MySQLAccess
{
private:
    sql::Connection *conn;
public:
    MySQLAccess();
    ~MySQLAccess();
    sql::Connection* GetHandle();
};

#endif
