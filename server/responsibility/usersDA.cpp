#include "usersDA.h"
#include "MySQLAccess.h"
#include "../models/user.h"


#include "../include/mysql/jdbc.h"
#include <string>

using namespace std;

usersDA::usersDA() {}

usersDA::~usersDA() {}

user* usersDA::selectAllUsers()
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::Statement *stmt;
    sql::ResultSet *res;

    user *users;
    int numOfUsers;

    stmt = conn->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM users;");
    while (res->next())
    {
        numOfUsers = res->getInt(1);
    }

    users = new user[numOfUsers];
    res = stmt->executeQuery("SELECT * FROM users;");
    for (int i=0; i<numOfUsers; i++)
    {
        if (res->next())
        {
            users[i].setUsername(res->getString(1));
            users[i].setPassword(res->getString(2));

            string state = res->getString(3);

            if (state.compare("") == 0)
                users[i].setState( 0, state );
            else if (state.find("ROOM ") == 0)
                users[i].setState( 2, state.substr(5) );
            else
                users[i].setState( 1, state );

            users[i].setRole(res->getInt(4));
        }
    }

    //delete res;
    //delete stmt;
    //delete conn;

    return users;
}

user* usersDA::findUserWithName(string name)
{
    usersDA* myUsersDA = new usersDA();

    int numOfUsers = myUsersDA->countUserWithName(name);

    //delete myUsersDA;

    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;
    sql::ResultSet *res;

    user *users;
    users = new user[numOfUsers];

    stmt = conn->prepareStatement("CALL findUserWithName(?);");
    stmt->setString(1, name);
    res = stmt->executeQuery();


    for (int i=0; i<numOfUsers; i++)
    {
        if (res->next())
        {
            users[i].setUsername(res->getString(1));
            users[i].setPassword(res->getString(2));

            string state = res->getString(3);

            if (state.compare("") == 0)
                users[i].setState( 0, state );
            else if (state.find("ROOM ") == 0)
                users[i].setState( 2, state.substr(5) );
            else
                users[i].setState( 1, state );

            users[i].setRole(res->getInt(4));
        }
    }

    //delete res;
    //delete stmt;
    //delete conn;

    return users;
}

int usersDA::countAllUsers()
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::Statement *stmt;
    sql::ResultSet *res;

    int numOfUsers;

    stmt = conn->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM users;");
    while (res->next())
    {
        numOfUsers = res->getInt(1);
    }

    //delete res;
    //delete stmt;
    //delete conn;

    return numOfUsers;
}

int usersDA::countUserWithName(string name)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;
    sql::ResultSet *res;

    int numOfUsers;

    stmt = conn->prepareStatement("CALL countUserWithName(?);");
    stmt->setString(1, name);
    res = stmt->executeQuery();

    while (res->next())
    {
        numOfUsers = res->getInt(1);
    }

    //delete res;
    //delete stmt;
    //delete conn;

    return numOfUsers;
}

void usersDA::updateState(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL SetState(?, ?, ?);");
    stmt->setString(1, usr.getUsername());
    stmt->setInt(2, usr.getState().type);
    stmt->setString(3, usr.getState().name);

    stmt->execute();

    //delete stmt;
    //delete conn;
}

void usersDA::clearState(user usr)
{
    cout<<"before ex 01 "<<usr.getUsername()<<endl;
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL ClearState(?);");
    stmt->setString(1, usr.getUsername());
    cout<<"before ex "<<usr.getUsername()<<endl;
    stmt->execute();

    //delete stmt;
    //delete conn;
}

bool usersDA::validateLogin(string username, string password)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;
    sql::ResultSet *res;

    stmt = conn->prepareStatement("SELECT ValidateLogin(?, ?);");
    stmt->setString(1, username);
    stmt->setString(2, password);
    res = stmt->executeQuery();

    bool result;

    while (res->next())
        result = res->getBoolean(1);

    //delete res;
    //delete stmt;
    //delete conn;

    return result;
}

void usersDA::insertUser(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("INSERT INTO users (username, password, role) VALUES (?, ?, ?);");
    stmt->setString(1, usr.getUsername());
    stmt->setString(2, usr.getPassword());
    stmt->setInt(3, usr.getRole());

    stmt->execute();

    //delete stmt;
    //delete conn;
}

void usersDA::modifyUser(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL modifyUser(?, ?, ?);");
    stmt->setString(1, usr.getUsername());
    stmt->setString(2, usr.getPassword());
    stmt->setInt(3, usr.getRole());

    stmt->execute();

    //delete stmt;
    //delete conn;
}

void usersDA::deleteUser(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL deleteUser(?);");
    stmt->setString(1, usr.getUsername());

    stmt->execute();

    //delete stmt;
    //delete conn;
}
