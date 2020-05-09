#ifndef DB_ACCESS_H
#define DB_ACCESS_H

#include "../responsibility/usersDA.h"
#include "../responsibility/roomsDA.h"
#include "../responsibility/messagesDA.h"
#include "../responsibility/rolesDA.h"
#include <map>

using namespace std;

void mapUser(map<user,int> &myMap);

void mapRoom(map<string, Room> &myMap);

void updateState(user usr);

void clearState(user usr);

void saveMessage(messageSend msg, string receiver);

messageSend* getMessageOfUser(user usr);

int countMessageOfUser(user usr);

void deleteMessageOfUser(user usr);

bool haveOfflineMessage(user usr);

void insertUser(user usr);

void modifyUserDB(user usr);

void deleteUserDB(user usr);

string getAllRoles();

void insertRoom(Room room);

void modifyRoomDB(Room room);

void deleteRoomDB(Room room);

user* findUserWithName(string name);

int countUserWithName(string name);

#endif//DB_ACCESS_H
