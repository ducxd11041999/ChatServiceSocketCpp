#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string.h>
#define roomIDSize    20

using namespace std;

class Room
{
    private:
        int capacity, availableUsers;
        int *fdRoomMembers;
        char roomID[roomIDSize];
    public:
        int add(int fdUser);
        void removeUser(int fdUser);
        void setRoomID(char RoomID[]);
        void setCapacity(int Capacity);
        void setRoom(char RoomID[], int Capacity);
        int *listClients();
        char *getRoomID();
        int getCapacity();
        int getOnlUser();
        bool find(int fdUser);

        /* --- Constructors & Deconstructors --- */
        Room();
        Room(char RoomID[], int Capacity);

};

#endif // ROOM_H
