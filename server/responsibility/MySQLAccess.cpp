#include "MySQLAccess.h"
#include "../config/config.h"


#include "../include/mysql/jdbc.h"


MySQLAccess::MySQLAccess() {}

MySQLAccess::~MySQLAccess() {}

sql::Connection* MySQLAccess::GetHandle()
{
   
     try
     {
        sql::Driver * driver = sql::mysql::get_driver_instance();
        conn = driver->connect(URI, USER, PASS);
        conn->setSchema(DB);
        return conn;
     }
    catch (sql::SQLException &e)
    {
        cout<<"Sorry can't connection with database"<<endl;
        exit(0);
    }
   
}
