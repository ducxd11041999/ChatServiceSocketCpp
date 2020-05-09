#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H

#include "../config/config.h"


// i need to move enum at here because my struct with need it
enum	eType
{
    nowork,
	loginn,
	withh,
	joinn,
	sendMess,
	loginSuccess,
	loginFail,
	withSuccess,
	withFail,
	joinSuccess,
	joinFail,		//10
	sendSuccess,
	sendFail,
	terminateChat,
	failTerminate,
	successTerminate,
	showRoom,
	allRoom,
	showUsers,
	createUser,
	createSuccess,		//20
	createFail,
	modifyUser,
	modifySuccess,
	modifyFail,
	createRoom,
	modifyRoom,
	getRoles,
	showRoles,
    deleteUser,
    deleteSuccess,		// 30
    deleteFail,
    deleteRoom,
	getuser,
	finduser,
	onlineuser,
	offlineuser,
	checkparty,
};


// SUPORT FOR LOGIN FUNCTION
struct login
{
        char uName[SIZE_U];
        char pWord[SIZE_P];
        char state[SIZE_U]; //ONLY FOR SENDING USER STATE WHEN RECONNECTING
        int role;
};



// SUPORT FOR WITH FUNCTION
struct with
{
        char userRev[SIZE_U];
        eType userStatus;

};

// SUPORT FOR JOIN ROOM FUNCTION
 struct join
{
        char roomName[SIZE_U];

};

// SUPORT FOR ROOM FUNCTION
struct roomM
{
    char roomID[SIZE_U];
    int capacity;
};



#endif//COMMONSTRUCT_H
