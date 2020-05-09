#ifndef MESSAGE_DA_H
#define MESSAGE_DA_H

#include "MySQLAccess.h"
#include "../models/message.h"
#include "../models/user.h"

using namespace std;

class messagesDA
{
public:
    messagesDA();
    ~messagesDA();
    void saveMessage(messageSend msg, string receiver);
    messageSend* getMessageOfUser(user usr);
    void deleteMessageOfUser(user usr);
    int countMessageOfUser(user usr);
    bool haveOfflineMessage(user usr);

private:
    MySQLAccess my_conn;
};

#endif//MESSAGE_DA_H
