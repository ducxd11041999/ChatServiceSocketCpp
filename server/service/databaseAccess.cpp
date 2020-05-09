#include "databaseAccess.h"
#include "../models/user.h"

void mapUser(map<user, int> &myMap)
{
    usersDA* myUsersDA = new usersDA();

    int numOfUser = myUsersDA->countAllUsers();
    user *users = new user[numOfUser];
    users = myUsersDA->selectAllUsers();

    delete myUsersDA;

    for (int i=0; i<numOfUser; i++)
    {
        myMap.insert(pair< user , int > (users[i],-1));
    }

    users = NULL;
    delete users;

}

void mapRoom(map<string, Room> &myMap)
{
    roomsDA* myRoomsDA = new roomsDA();

    int numOfRoom = myRoomsDA->countAllRooms();
    Room *rooms;
    rooms = myRoomsDA->selectAllRooms();

    delete myRoomsDA;

    for (int i=0; i<numOfRoom; i++)
    {
        myMap.insert(pair<string, Room> (rooms[i].getRoomID(), rooms[i]));
    }

    rooms = NULL;
    delete rooms;
}

void updateState(user usr)
{
    usersDA* myUsersDA = new usersDA();

    myUsersDA->updateState(usr);

    delete myUsersDA;
}

void clearState(user usr)
{
    usersDA* myUsersDA = new usersDA();

    myUsersDA->clearState(usr);

    delete myUsersDA;
}

void saveMessage(messageSend msg, string receiver)
{
    messagesDA* myMessagesDA = new messagesDA();

    myMessagesDA->saveMessage(msg, receiver);

    delete myMessagesDA;
}

messageSend* getMessageOfUser(user usr)
{
    messageSend *messageArr;

    messagesDA* myMessagesDA = new messagesDA();

    messageArr = new messageSend[myMessagesDA->countMessageOfUser(usr)];

    messageArr = myMessagesDA->getMessageOfUser(usr);

    delete myMessagesDA;

    return messageArr;

}

int countMessageOfUser(user usr)
{
    messagesDA* myMessagesDA = new messagesDA();

    int numOfMess = myMessagesDA->countMessageOfUser(usr);

    delete myMessagesDA;

    return numOfMess;
}

void deleteMessageOfUser(user usr)
{
    messagesDA* myMessagesDA = new messagesDA();

    myMessagesDA->deleteMessageOfUser(usr);

    delete myMessagesDA;
}

bool haveOfflineMessage(user usr)
{
    messagesDA* myMessagesDA = new messagesDA();

    bool result = myMessagesDA->haveOfflineMessage(usr);

    delete myMessagesDA;

    return result;
}

/**FUNCTION FOR ADMIN*/
void insertUser(user usr)
{
    usersDA* myUsersDA = new usersDA();

    myUsersDA->insertUser(usr);

    delete myUsersDA;
}

void modifyUserDB(user usr)
{
    usersDA* myUsersDA = new usersDA();

    myUsersDA->modifyUser(usr);

    delete myUsersDA;
}

void deleteUserDB(user usr)
{
    usersDA* myUsersDA = new usersDA();

    myUsersDA->deleteUser(usr);

    delete myUsersDA;
}

string getAllRoles()
{
    rolesDA* myRolesDA = new rolesDA();

    return myRolesDA->getAllRoles();
}

void insertRoom(Room room)
{
    roomsDA* myRoomDA = new roomsDA();

    myRoomDA->insertRoom(room);

    delete myRoomDA;
}

void modifyRoomDB(Room room)
{
    roomsDA* myRoomDA = new roomsDA();

    myRoomDA->modifyRoom(room);

    delete myRoomDA;
}

void deleteRoomDB(Room room)
{
    roomsDA* myRoomDA = new roomsDA();

    myRoomDA->deleteRoom(room);

    delete myRoomDA;
}

user* findUserWithName(string name)
{
    usersDA* myUsersDA = new usersDA();

    int numOfUser = myUsersDA->countUserWithName(name);

    user *users = new user[numOfUser];
    users = myUsersDA->findUserWithName(name);

    delete myUsersDA;

    return users;

}

int countUserWithName(string name)
{
    usersDA* myUsersDA = new usersDA();

    int numOfUser = myUsersDA->countUserWithName(name);

    delete myUsersDA;

    return numOfUser;

}
