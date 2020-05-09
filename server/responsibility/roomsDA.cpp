#include "roomsDA.h"
#include "MySQLAccess.h"
#include "../models/Room.h"
#include "../Util/util.h"

#include <iostream>
#include "../include/mysql/jdbc.h"
#include <string>
#include <cstring>

using namespace std;

roomsDA::roomsDA() {}

roomsDA::~roomsDA() {}

Room* roomsDA::selectAllRooms()
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::Statement *stmt;
    sql::ResultSet *res;

    Room *rooms;
    int numOfRooms;

    stmt = conn->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM rooms;");
    while (res->next())
    {
        numOfRooms = res->getInt(1);
    }
    rooms = new Room[numOfRooms];
    res = stmt->executeQuery("SELECT * FROM rooms;");
    for (int i=0; i<numOfRooms; i++)
    {
        if (res->next())
        {
            rooms[i].setRoomID(strToChar(res->getString(1)));
            rooms[i].setCapacity( int (res->getInt(2)) );
        }
    }

    delete conn;
    return rooms;
}

int roomsDA::countAllRooms()
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::Statement *stmt;
    sql::ResultSet *res;

    int numOfRooms;

    stmt = conn->createStatement();
    res = stmt->executeQuery("SELECT COUNT(*) FROM rooms;");
    while (res->next())
    {
        numOfRooms = res->getInt(1);
    }

    delete res;
    delete stmt;
    delete conn;

    return numOfRooms;
}

void roomsDA::insertRoom(Room room)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("INSERT INTO rooms (roomID, capacity) VALUES (?, ?);");
    stmt->setString(1, room.getRoomID());
    stmt->setInt(2, room.getCapacity());

    stmt->execute();

    delete stmt;
    delete conn;
}

void roomsDA::modifyRoom(Room room)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL modifyRoom(?, ?);");
    stmt->setString(1, room.getRoomID());
    stmt->setInt(2, room.getCapacity());

    stmt->execute();

    delete stmt;
    delete conn;
}
void roomsDA::deleteRoom(Room room)
{
    sql::Connection* conn = my_conn.GetHandle();
    sql::PreparedStatement *stmt;

    stmt = conn->prepareStatement("CALL deleteRoom(?);");
    stmt->setString(1, room.getRoomID());

    stmt->execute();

    delete stmt;
    delete conn;
}
