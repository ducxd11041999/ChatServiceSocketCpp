#include "messagesDA.h"
#include "MySQLAccess.h"
#include "../models/message.h"
#include "../include/mysql/jdbc.h"
#include "../Util/util.h"

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

messagesDA::messagesDA() {}

messagesDA::~messagesDA() {}

void messagesDA::saveMessage(messageSend msg, string receiver)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL saveMessage(?, ?, ?, ?);");
    stmt->setString(1, msg.sender);
    stmt->setString(2, receiver);
    stmt->setString(3, msg.mess);
    stmt->setDouble(4, (double) msg.sendTime);
    stmt->execute();

    delete stmt;
    delete conn;
}

messageSend* messagesDA::getMessageOfUser(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;
    sql::ResultSet *res;

    messageSend *messageArr;
    int numOfMessages;

    stmt = conn->prepareStatement("CALL countMessageOfUser(?);");
    stmt->setString(1, usr.getUsername());
    res = stmt->executeQuery();

    while (res->next())
        numOfMessages = res->getInt(1);

    if (numOfMessages==0)
        return nullptr;

    conn->close();
    conn = my_conn.GetHandle();
    messageArr = new messageSend[numOfMessages+1];

    sql::PreparedStatement *stmt2;
    sql::ResultSet *res2;
    stmt2 = conn->prepareStatement("CALL getMessageOfUser(?);");
    stmt2->setString(1, usr.getUsername());
    res2 = stmt2->executeQuery();

    for (int i=0; i<numOfMessages; i++)
    {
       // cout << "sol uong la: " <<i << endl;
        if (res2->next())
        {
            strcpy(messageArr[i].sender, strToChar(res2->getString(1)));
            strcpy(messageArr[i].mess, strToChar(res2->getString(2)));
            messageArr[i].sendTime = res2->getDouble(3);
        }
    }

    delete res;
    delete stmt;
    delete res2;
    delete stmt2;
    delete conn;

    return messageArr;
}

void messagesDA::deleteMessageOfUser(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL deleteMessageOfUser(?);");
    stmt->setString(1, usr.getUsername());
    stmt->execute();

    delete stmt;
    delete conn;
}

int messagesDA::countMessageOfUser(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;
    sql::ResultSet *res;

    int numOfMessages;

    stmt = conn->prepareStatement("CALL countMessageOfUser(?);");
    stmt->setString(1, usr.getUsername());
    res = stmt->executeQuery();

    while (res->next())
        numOfMessages = res->getInt(1);

    delete res;
    delete stmt;
    delete conn;

    return numOfMessages;
}

bool messagesDA::haveOfflineMessage(user usr)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;
    sql::ResultSet *res;

    stmt = conn->prepareStatement("SELECT haveOfflineMessage(?);");
    stmt->setString(1, usr.getUsername());
    res = stmt->executeQuery();

    bool result;

    while (res->next())
        result = res->getBoolean(1);

    delete res;
    delete stmt;
    delete conn;

    return result;
}
