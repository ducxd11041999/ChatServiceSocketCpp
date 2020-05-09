#include "config.h"
#include "../models/package.h"
#include "../models/user.h"
class client
{
	private:
		int clientSocket;
		struct  sockaddr_in serverAddr;
		pthread_t thread;

	public:
		user User;
		void initSocket();
		struct sockaddr_in* getServerAddr();
		void withHandler(string,color&);
		void commandHandler();
		void Excecute();
		void roomHandler(string , color &);
		char *Clear_opt(char * mess);
		char *detectmess(char* mess, color &option);
		char *changeColor(char *mess, color option);

		int connectServer();
		int getClientSocket();
		void modifyRoomHandler();
		void modifyHandler();
		void showAllRoles();

		void createNewUser();
		void createNewRoom();

		void clearUser();
		void removeRoom();

		void getAllUser();
		void findUser(string name);

		static void inputLine();
		static void showMenu(user*);

		// Constructor
		client();
};

void receiverHandler(package pkrecv, user *User);

