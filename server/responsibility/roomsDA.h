#ifndef ROOM_DA_H
#define ROOM_DA_H

#include "MySQLAccess.h"
#include "../models/Room.h"

using namespace std;

class roomsDA
{
public:
    roomsDA();
    ~roomsDA();
    Room* selectAllRooms();
    int countAllRooms();
    void insertRoom(Room room);
    void modifyRoom(Room room);
    void deleteRoom(Room room);

private:
    MySQLAccess my_conn;
};

#endif

