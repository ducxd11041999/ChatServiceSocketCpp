#ifndef SOCK_CONNECT_H
#define SOCK_CONNECT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <errno.h>
#include <limits.h>
#include <ctime>
#include <chrono>
#include <iostream>
using namespace std;
#include "../Util/util.h"
#include "../models/Room.h"
#include "../models/user.h"
#include "../models/package.h"
#include "../config/config.h"
#define POLL_SIZE 35	// fd max ( user max ) can connect server
#define LOCAL_HOST_SOCK 54000  // local host for socket
#define MAXLINE 4096
#define MAXROOM 10
class sock_connect {
private:
	int listenfd, connfd, sockfd, maxi,i;
	int nready;
	ssize_t n;
	struct pollfd client_fd[POLL_SIZE];
	socklen_t clilen;
	struct sockaddr_in server_address,client_address;

	std::map< user,int> mapuser;
	std::map<string, Room> roomMap;
	Room room_array[MAXROOM];

	package pakSen;
	char buf[MAXLINE];
public:
	void Init(); // create a socket, poll structure
    void EventServer(); // manager all event in server socket include accept connect from a client
    void EventClient(); // manager all event from client socket include read message
	void Login(int client_fd,package *pk); //login


    void Execute(); //just test but can use it's structure fod do the socket connect later
	//void sock_connect::SendRooms(int clientfd, Room listroom[]);
	std::map< user,int>  // get map user
	GetMapfd()
	{
		return mapuser;
	}
	void analyzePackage(int client_fd,package *pk);


	// For user
	void WithFunction(int client_fd,package *pk);//with change state
	void OneToOneChat(int client_fd,package *pk);
    void LeaveChatUser(int client_fd,package *pk);
    void FindUser(int client_fd, package *pk);

    // For admin
    void AddUser(int client, package *pk);
    bool CheckAdmin(int fd);
    void addRoom(int client_fd, package *pk);
    void handleModifyUser(int client, package *pk);
    void ModifyRoom(int client_fd, package *pk);
    void DeleteRoom(int client_fd, package *pk);
    void DeleteUser(int client_fd, package *pk);

    // For room
    void JoinRoom(int client_fd,package*pk);
    void RoomChat(int client_fd,package*pk, user u);
    void LeaveRoom(int client_fd, user u);
    void ShowRoom(int client_fd);
    void getUserOnline(int client_fd,package *pk);


    bool changeStateUserOff(int fd);

    void handleOfflineMessage(int client_fd); // Get offline message

    //notification offline user
    void notificationOffline(int fd,eType e);

	user findUserByfd(int fd);
	int getfdUsername(string username);
	bool checkfd(int);

	//get roles and send for client
	void getListRoles(int client_fd,package*pk);

};
#endif // SOCK_CONNECT_H


