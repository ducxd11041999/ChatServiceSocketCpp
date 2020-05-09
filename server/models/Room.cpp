#include "Room.h"

int Room::add(int fdUser)
{
    if (availableUsers < capacity)
    {
        for (int i = 0; i < availableUsers; i++)
        {
            if (fdRoomMembers[i] == fdUser)
            {
                cerr << "User " << fdRoomMembers << " already in room" << endl;
                return -1;
            }
        }
        fdRoomMembers[availableUsers] = fdUser;
        availableUsers++;
        return availableUsers;
    }
    else
        cerr << "Room is full" << endl;
        return -2;
}

void Room::removeUser(int fdUser)
{
    for (int i = 0; i < availableUsers; i++)
    {
        if (fdRoomMembers[i] == fdUser)
        {
            for (int j = i; j < availableUsers; j++)
            {
                fdRoomMembers[j] = fdRoomMembers[j+1];
            }
            availableUsers--;
            break;
        }
    }
}


void Room::setRoomID(char RoomID[])
{
    // wipe all data
    memset(this->roomID, '\0', roomIDSize);
    // Add new data
    for (int i = 0; i < (int) strlen(RoomID); i++)
    {
        this->roomID[i] = RoomID[i];
    }
}

void Room::setCapacity(int Capacity)
{
    if (Capacity < availableUsers)
        cerr << "New capacity is smaller than available users" << endl;
    else
    {
        // Change to array with new size;
        int *tempfdRoomMember = new int[Capacity];
        for (int i = 0; i < availableUsers; i++)
        {
            tempfdRoomMember[i] = fdRoomMembers[i];
        }

        // Swap array
        if (fdRoomMembers != NULL)
        {
            cout<<"Delete in setCapacity"<<endl;
            delete []fdRoomMembers;
        }
        fdRoomMembers = tempfdRoomMember;
        tempfdRoomMember = NULL;
        // Update new capacity for private member
        this->capacity = Capacity;
    }
}

void Room::setRoom(char RoomID[], int Capacity)
{
    setRoomID(RoomID);
    setCapacity(Capacity);
}

int *Room::listClients()
{
    // Craete new memory space and copy data in
    int *tempMemberInRoom = new int[availableUsers];
    for (int i = 0; i < availableUsers; i++)
    {
        tempMemberInRoom[i] = fdRoomMembers[i];
    }
    // return address
    return tempMemberInRoom;
}


char *Room::getRoomID()
{
    // Craete new memory space and copy data in
    char *tempRoomID = new char[roomIDSize];
    for (int i = 0; i < roomIDSize; i++)
    {
        tempRoomID[i] = roomID[i];
    }
    // return address
    return tempRoomID;
}

int Room::getCapacity()
{
    return capacity;
}

int Room::getOnlUser()
{
    return availableUsers;
}

bool Room::find(int fdUser)
{
    for (int i = 0; i < availableUsers; i++)
    {
        if (fdRoomMembers[i] == fdUser)
        {
            return true;
        }
    }
    return false;
}

Room::Room()
{
    availableUsers = 0;
    capacity = 0;
    fdRoomMembers = NULL;
}

Room::Room(char RoomID[], int Capacity)
{
    availableUsers = 0;
    capacity = 0;
    fdRoomMembers = NULL;

    // check and set RoomID
    if (strlen(RoomID) == 0) {
        cout << "Set room ID fail, empty string"<< endl;
    }
    else {
        for (int i = 0; i < (int) strlen(RoomID); i++)
        {
            this->roomID[i] = RoomID[i];
        }
    }

    // check and set room capacity
    if (Capacity < 0) {
        cout << "Set capacity fail, Capacity must greater than 0" << endl;
    }
    else {
        this->capacity = Capacity;
        fdRoomMembers = new int[capacity];
    }
}
