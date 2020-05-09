#include "client.h"
#include "../Util/util.h"
#include <iomanip>


/* ------------------------------------ GLOBAL ARGUMENTS ------------------------------------*/
// thread
pthread_t thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t condx = PTHREAD_COND_INITIALIZER;
// basic arguments
volatile int flag_check = FLAG_OFF;
bool flag_w = false;
bool flag_j = false;
bool flag_admin = false;
volatile bool flag_w_online = false;
string userw,roomw;;
// struct
struct threadArguments
{
	int clientSocket;
	struct  sockaddr_in *serverAddr;
	user *User;
} doReceivingThread;


/* ------------------------------------ GLOBAL FUNCTIONS ------------------------------------*/
bool sendPackage(int clientSocket, package pkg)
{
	if(flag_check==FLAG_OFF)
	{
		send(clientSocket,&pkg, sizeof(package), 0);
		return true;
	}
	else
		cout<<"Server is not found"<<endl;
	return false;
}

bool loginHandler(int clientSocket)
{
	package pkg;
	char username[SIZE_U];
	char password[SIZE_P];

	string s;
	do
	{
		cout << "Enter username: ";
		getline(cin, s);
		if (s.size()>SIZE_U)
		{
			cout << "\tOut size of username ! Please enter again !" << endl;
			continue;

		}
		copy (s.begin(), s.begin()+SIZE_U, username);
        //username[SIZE_U+1] = '\0';

	}
	while (s.size()>SIZE_U);

	do
	{
		cout << "Enter password: ";
		getline(cin, s);
		if (s.size()>SIZE_P)
		{
			cout << "\tOut size of password! Please enter again !"<< endl;
			continue;
		}
		copy (s.begin(), s.begin()+SIZE_P, password);
        //password[SIZE_P+1] = '\0';

	}
	while (s.size()>SIZE_P);

	pkg.setType(loginn);
	pkg.setUP(username, password);
	if (sendPackage(clientSocket,pkg))
		return true;
	return false;
}

void deletePosition(char str[], int pos)
{
	for(int i = 0; i< (int) strlen(str); i++)
	{
		str[i] = str[i+1];

	}
	str[strlen(str)] = '\0';
}

int reconnectServer(int clientSocket, struct sockaddr_in *serverAddr, int &time)
{
    // wait until old socket is closed
	while (close(clientSocket));
    // recreate socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		cout<<endl<<"[SERVER DISCONNECTED] Create socket failed" << endl;
		cout<<"Exit program!" << endl;
		pthread_exit(NULL);
		pthread_cancel(thread);
		exit(1);
		return 1;
	}
    // reconnect
	int rc;
	rc = connect(clientSocket, (struct sockaddr*) serverAddr, sizeof(*serverAddr));
    if (rc < 0)	// Connect fail, wait 1s and reconnect
    {
    	usleep(1000000);
    }
    else		// Connect successed
    {
    	return 0;
    }
    return 1;
}

/* ------------------------------------ RECEIVE THREAD ------------------------------------*/
void * doRecieving(void * arguForThisThread)
{
	pthread_mutex_trylock(&mutex);
	struct threadArguments *args = (struct threadArguments *) arguForThisThread;
	int clientSocket = args->clientSocket;
	struct  sockaddr_in *serverAddr = args->serverAddr;
	user *User = args->User;

    //printf("Create Thread");
	while(1)
	{
		package pkrecv;
		int read = recv(clientSocket,&pkrecv,sizeof(package),0);
		flag_check = FLAG_OFF;
        // check server is online or offline
		if (read==0)
		{
			flag_check = FLAG_ON;
			int disconnectTime = 0;

            // wait reconnect to server
			while(reconnectServer(clientSocket, serverAddr, disconnectTime));
            // Reconnected
			flag_check = FLAG_OFF;
			if (User->flag_l == true)
			{
				package pkg;
				pkg.setType(loginn);
				pkg.setUP(User->getUsername(), User->getPassword());
				sendPackage(clientSocket, pkg);
			}
		}

        else if(read < 0 )
        {
        	cout<<"Err receiver package from server"<<endl;
        }
        else
        {
        	receiverHandler(pkrecv, User);
       	}

        //pthread_mutex_unlock(&mutex);
	}
}

void receiverHandler(package pkrecv, user *User)
{
	message m = pkrecv.showAction();
	int type = pkrecv.getType();
	struct tm *messTime = localtime(&m.MessSend.sendTime);
	int day = messTime->tm_mday;
	// month is counted from 0
	int month = messTime->tm_mon + 1;
	// year is counted from 1900
	int year = messTime->tm_year + 1900;
	//cout<<type<<endl; debug case number
	switch(type)
	{
		case nowork:
			cout<<"No type is set"<<endl;
		break;
		case joinn:
		cout<<"\t\tJOIN ROOM"<<endl;
		cout<<"ROOM ID : "<<m.Join.roomName<<endl;
		break;

		case joinSuccess:
		cout<<m.MessSend.mess<<endl;
		flag_w = false;
		flag_j = true;
            //TODO: handle success
		pthread_cond_signal(&cond);
		break;

		case joinFail:
            //cout<<"joinFail"<<endl;
		cout<<m.MessSend.mess<<endl;
		pthread_cond_signal(&cond);
		break;

		case allRoom:
		cout<<m.MessSend.mess<<endl;
		pthread_cond_signal(&cond);
		break;

		case sendMess:
		{
			if (strcmp(m.MessSend.sender, (char*)"System") == 0)
			{
                cout<<"\r"<<m.MessSend.mess<<endl;
			}
			else if (strcmp(m.MessSend.sender, User->getUsername()) != 0)
			{
                cout<<"\r                                       ";
                cout<<"\t\t["<<day<<"/"<<month<<"/"<<year<< " "<<messTime->tm_hour<<":"<<messTime->tm_min<<"] "<<m.MessSend.sender<<": "<<m.MessSend.mess<<endl;
			}

			else
			{
                cout<<"\033[1A\r                                ";
                cout<<"\r["<<day<<"/"<<month<<"/"<<year<< " "<<messTime->tm_hour<<":"<<messTime->tm_min<<"] "<<m.MessSend.sender<<": "<<m.MessSend.mess<<endl;
			}

			client::inputLine();
			pthread_cond_signal(&cond);
			break;
		}

		case loginSuccess:

		//cout<<"[DEBUG] role :"<<m.Login.role<<endl;//[DEBUG]
		//cout<<"[DEBUG] username :"<<m.Login.uName<<endl;//[DEBUG]
		//cout<<"[DEBUG] password :"<<m.Login.pWord<<endl;//[DEBUG]
		User->setUsername(m.Login.uName);
		User->setPassword(m.Login.pWord);
		User->setState(m.Login.state);
		User->setroles(m.Login.role);

		if (User->flag_l == false)
		{
			client::showMenu(User);
		}
		User->flag_l = true;
            //TODO: create user
		pthread_cond_signal(&cond);
		break;

		case loginFail:
		cout<<"\t\t"<<m.MessSend.mess<<endl;
		pthread_cond_signal(&cond);
		break;

		case sendFail:
		cout<<"\r                             "<<m.MessSend.mess<<endl;
		client::inputLine();
		pthread_cond_signal(&cond);
		break;

		case createSuccess:
			cout<<"\t\t"<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
			break;

		case createFail:
			cout<<"\t\t"<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
			break;

		case showUsers:
			cout<<"\t\t"<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
			break;

		case deleteSuccess:
			cout<<"\t\t"<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
			break;

		case deleteFail:
			cout<<"\t\t"<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
			break;

		case withSuccess:
		if (m.With.userStatus==onlineuser)
		{
			flag_w_online = true;
			cout<<"Connection with "<<userw<<endl;
		}
		else
		{
            flag_w_online = false;
            cout<<"Send offline msgs to username: "<<userw<<endl;
		}
		flag_w = true;
		flag_j = false;
            //TODO: handle success
		pthread_cond_signal(&cond);
		break;
		case checkparty:
		if (m.With.userStatus==onlineuser)
		{
			flag_w_online = true;
			cout<<"\r";
			cout<<"user "<<userw<<" is now online                           "<<endl;
			client::inputLine();
		}
		else
		{
            flag_w_online = false;
            cout<<"\r";
            cout<<"user "<<userw<<" is now offline                          "<<endl;
            client::inputLine();
		}
		break;

		case withFail:
		cout<<"\t\t"<<m.MessSend.mess<<endl;
		pthread_cond_signal(&cond);
		break;

		case successTerminate:
		client::showMenu(User);
		flag_w = false;
		flag_j = false;
		client::inputLine();

		//cout<<"Back success"<<endl;
		pthread_cond_signal(&cond);
		break;

		case showRoles:
			cout<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
		break;
		case modifySuccess:
			cout<<"\t\t"<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
		break;
		case modifyFail:
			cout<<"\t\t"<<m.MessSend.mess<<endl;
			pthread_cond_signal(&cond);
		break;
		default:
		cout<<"This type is haven't config"<<endl;
		cout<<"Message is: "<<m.MessSend.mess<<endl;
		break;
	}
}



/* ------------------------------------ CLASS FUNCTIONS ------------------------------------*/
void client::initSocket()
{
    /*init info server*/
	this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(PORT);
	this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

int client::connectServer()
{
	int rc = connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
	if(rc)
	{
		cout<<"Waiting for server online..."<<endl;
		while(rc)
		{
			usleep(1000000);
			rc = connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
		}
	}

	cout<<"Connection established!"<<endl;
    // Add data into a struct for thread
	doReceivingThread.clientSocket = getClientSocket();
	doReceivingThread.serverAddr = getServerAddr();
	doReceivingThread.User = &User;
    // create thread
	pthread_create(&thread, NULL, doRecieving, (void *) &doReceivingThread );
	return 1;
}

void client::Excecute()
{
	color option = Default;
	while(1)

	{
		if(!User.flag_l)
		{
			if (loginHandler(clientSocket))
			{
                // if sent login message wait for listen thread return result
				pthread_cond_wait(&cond, &mutex);
			}
		}
		else
		{
			commandHandler();
            //cout<<"under command flag with"<<flag_w<<endl;//[debug]
			if (!flag_w && !flag_j)
				pthread_cond_wait(&cond, &mutex);
			withHandler(userw,option);
			roomHandler(roomw,option);
		}
	}

	pthread_join(thread,NULL);
	pthread_cancel(thread);
}

void client::roomHandler(string roomw, color &option)
{
	char mes[SIZE_MESS] = {};
	if(flag_j)
	{
		package newpk;
		inputLine();
		newpk.setType(sendMess);
		while (fgets(mes, SIZE_MESS, stdin) != NULL)
		{
			mes[strlen(mes)-1] = '\0';
			if(strstr(mes, "?back")!=NULL)
			{
				newpk.setType(terminateChat);
				newpk.setMess(strToChar(roomw));
				sendPackage(clientSocket, newpk);
            	pthread_cond_wait(&cond, &mutex);
            	break;
			}
			if (strlen(mes) == 0)
			{
				cout<<"\033[1A\r";
				inputLine();
			}
			else
			{
				strcpy(mes , detectmess(mes,option));
				newpk.setMess(mes);
				sendPackage(clientSocket,newpk);
				break;
			}
		}

        //pthread_mutex_unlock(&mutex);
	}
}

void client::commandHandler()
{
	package pkg;
	//char command[SIZE_MESS];
	string command;
	string cmd = "";
	while(!flag_w && !flag_j)
	{
		inputLine();
		getline(cin,cmd);
		while (cmd == "")
		{
            cout<<"\033[1A\r";
			inputLine();
            getline(cin,cmd);
		}

        command = cmd.substr(0, cmd.find(" "));

		if(command == "?with")
		{
			/*cin.ignore();
			getline(cin, userw);*/
			userw = cmd.substr(cmd.find(" ")+1, cmd.size());
			pkg.setType(withh);

			pkg.setRev(strToChar(userw));
			if(sendPackage(clientSocket,pkg))
				break;
		}
		else if(command=="?join")
		{
			/*cin.ignore();
			getline(cin, roomw);*/
			roomw = cmd.substr(cmd.find(" ")+1, cmd.size());
			pkg.setType(joinn);
			pkg.setRoom(strToChar(roomw));
			if (sendPackage(clientSocket, pkg))
				break;

		}
		else if( command == "?listroom")
		{
			pkg.setType(showRoom);
			if(flag_check==FLAG_OFF)
			{
				send(clientSocket,&pkg, sizeof(package), 0);
				break;
			}
		}
		else if(command == "?showusers")
        	{
                getAllUser();
        	}
        	else if (command ==  "?findusers")
        	{
                string name = cmd.substr(cmd.find(" ")+1, cmd.size());
                findUser(name);
        	}
		else if(User.getroles()==1) // function only for admin
		{
                    if(command ==  "?createuser")
                    {
                        createNewUser();
                    }
                    else if (command ==  "?modifyuser")
                    {
                        modifyHandler();
                    }
                    else if (command ==  "?deleteuser")
                    {
                        clearUser();
                    }
                    else if (command ==  "?createroom")
                    {

						createNewRoom();
                    }

                    else if (command ==  "?modifyroom")
                    {
                //cout<<"[DEBUG] midify room"<<endl;
                    	modifyRoomHandler();
                    }
                    else if (command ==  "?deleteroom")
                    {
                        removeRoom();
                    }
                    else
                        cout<<"Command not found "<<endl;

		}
		else
                cout<<"Command not found "<<endl;

	}
}



void client::withHandler(string userw, color &option)
{
	//pthread_mutex_trylock(&mutex);
	char mes[SIZE_MESS] = {};
	if(flag_w)
	{
		package newpk;
		inputLine();

		while (fgets(mes, SIZE_MESS, stdin) != NULL)
		{
            mes[strlen(mes)-1] = '\0';
            if(strstr(mes, "?back")!=NULL )      // Back successful
            {
            	newpk.setType(terminateChat);
                newpk.setMess(strToChar(userw));
            	sendPackage(clientSocket, newpk);
            	pthread_cond_wait(&cond, &mutex);
            	break;

            }
            else if (strlen(mes) == 0)
            {
            	cout<<"\033[1A\r";
            	inputLine();
            }
            else
            {
            	newpk.setType(sendMess);
            	strcpy(mes , detectmess(mes,option));
            	newpk.setMess(mes);
            	sendPackage(clientSocket, newpk);
            	break;
            }
        }

        //pthread_mutex_unlock(&mutex);

    }
}
char* client::changeColor(char *mess, color option)
{
	char *result;
	char *mau = new char[1024];

	switch (option)
	{
		case Red:
		strcpy(mau, "\033[0;31m");
		result = strcat(mau, mess);
		result = strcat(mau, "\033[0m");
		break;
		case Green:
		strcpy(mau, "\033[0;32m");
		result = strcat(mau, mess);
		result = strcat(mau, "\033[0m");
		break;
		case Blue:
		strcpy(mau, "\033[0;34m");
		result = strcat(mau, mess);
		result = strcat(mau, "\033[0m");
		break;
		case Yellow:
		strcpy(mau, "\033[0;33m");
		result = strcat(mau, mess);
		result = strcat(mau, "\033[0m");
		break;

		default:
		result = strcpy(mau, mess);
		break;

	}
	return result;
}
char* client::Clear_opt(char * mess)
{
	if( strlen(mess)==2)
	{
		mess[0]='\0';
		mess[1]='\0';
	}
	else if( strlen(mess)==3 )
	{
		mess[0] = mess[2];
		mess[1] = '\0';
	}
	else if( mess[2] ==' ')
	{
		mess[0] = mess[3];
		mess[1] =mess[4];

		for(int j =2; j <= (int)strlen(strToChar(mess)); j++)
		{
			mess[j] = mess[j+3];
		}
	}

	return mess;

}
char * client::detectmess(char* mess, color &option)
{
	if(mess[0] == '?' && (mess[1] =='R'|| mess[1] =='r') && (mess[2] == ' ' || mess[2] == '\0'|| mess[2] == '\n'))
	{
		option = Red ;
		mess = strcpy(strToChar(mess),Clear_opt(strToChar(mess)));
	}
	else if(mess[0] == '?' && (mess[1] =='G'|| mess[1] =='g')&& (mess[2] == ' '|| mess[2] == '\0'|| mess[2] == '\n'))
	{
		option = Green;
		mess = strcpy(strToChar(mess),Clear_opt(strToChar(mess)));
	}
	else if(mess[0] == '?' && (mess[1] =='B'|| mess[1] =='b')&& (mess[2] == ' '|| mess[2] == '\0'|| mess[2] == '\n'))
	{
		option = Blue;
		mess = strcpy(strToChar(mess),Clear_opt(strToChar(mess)));
	}
	else if(mess[0] == '?' && (mess[1] =='Y'|| mess[1] =='y')&& (mess[2] == ' '|| mess[2] == '\0'|| mess[2] == '\n'))
	{
		option = Yellow;
		mess = strcpy(strToChar(mess),Clear_opt(strToChar(mess)));
	}
	else if(mess[0] == '?' && (mess[1] =='D'|| mess[1] =='d')&& (mess[2] == ' '|| mess[2] == '\0'|| mess[2] == '\n'))
	{
		option = Default;
		mess = strcpy(strToChar(mess),Clear_opt(strToChar(mess)));
	}

	if (mess[0]!='\0')mess = changeColor(strToChar(mess),option);
	return mess;
}

void client::inputLine()
{
	if (!flag_w && !flag_j)
	{
		cout<<"\rEnter your command: ";
		fflush(stdout);
	}
	else if (flag_w)
	{
		if (flag_w_online)
			cout<<"\r@me<with>"<<userw<<"#: ";
		else
			cout<<"\r@me<with>"<<userw<<": ";
		fflush(stdout);
	}
	else if (flag_j)
	{
		cout<<"\r@me<with>"<<roomw<<": ";
		fflush(stdout);
	}
}

int client::getClientSocket()
{
	return clientSocket;
}

struct sockaddr_in *client::getServerAddr()
{
	return &serverAddr;
}

void client::showMenu(user * User)
{
	cout<<"\r"<<right<<setw(40)<<"MENU"<<endl;
	cout<< left << setw(30) <<"\t?with + <username> "
        << left << setw(40) <<"-> Chat with user 1v1 "<<endl;
    cout<< left << setw(30) <<"\t?join + <room name> "
        << left << setw(40) <<"-> Join a chat room "<<endl;
    cout<< left << setw(30) <<"\t?back "
        << left << setw(40) <<"-> Leave chat"<<endl;
    cout<< left << setw(30) <<"\t?listroom "
        << left << setw(40) <<"-> show list of available rooms"<<endl;
    cout<< left << setw(30) <<"\t?showusers "
        << left << setw(40) <<"-> show list of online users"<<endl;
    cout<< left << setw(30) <<"\t?findusers + <username> "
        << left << setw(40) <<"-> find all users with <username>"<<endl;
    cout<<endl;

	if(User->getroles()==1)
	{
        cout<<right <<setw(45)<<"Admin Functions"<<endl;
        cout<< left << setw(30) <<"\t?createuser "
            << left << setw(40) <<"-> create a lot of new user"<<endl;
        cout<< left << setw(30) <<"\t?modifyuser "
            << left << setw(40) <<"-> modify a user"<<endl;
        cout<< left << setw(30) <<"\t?deleteuser "
            << left << setw(40) <<"-> delete a user"<<endl;
        cout<< left << setw(30) <<"\t?createroom "
            << left << setw(40) <<"-> create a lot of new room"<<endl;
        cout<< left << setw(30) <<"\t?modifyroom "
            << left << setw(40) <<"-> modify a room"<<endl;
        cout<< left << setw(30) <<"\t?deleteroom "
            << left << setw(40) <<"-> delete a room"<<endl;
        cout<<endl;
	}
}

/* ------------------------------------ CONSTRUCTOR & DESTRUCTOR ------------------------------------*/
client::client()
{
    //pthread_create(&thread, NULL, client::doRecieving, (void *) &clientSocket );
    //pthread_create(&thread, NULL, doRecieving, (void *) &clientSocket );
	clientSocket = 0;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void client::modifyHandler()
{
	int Role;
	package pkModify;
	string userNameModify,passWordModify;
	//cin.ignore();
	do
		{
			cout<<"[Modify] Enter username : ";
			getline(cin, userNameModify);
			if(userNameModify.size()>SIZE_U)
			{
				cout << "\tOut size of username ! Please enter again !" << endl;
				userNameModify.clear();
				continue;

			}
			if(strcmp(strToChar(userNameModify),"?ignore")==0)return;
		}
    while (userNameModify.size()>SIZE_U);

	do
		{
			cout<<"[Modify] Enter new password of "<<userNameModify<<" : ";
			getline(cin, passWordModify);
			if(passWordModify.size()>SIZE_U)
			{
				cout << "\tOut size of username ! Please enter again !" << endl;
				passWordModify.clear();
				continue;

			}
			if(strcmp(strToChar(passWordModify),"?ignore")==0)return;
		}
    while (passWordModify.size()>SIZE_P);

	//showAllRoles();
	pkModify.setType(getRoles);
	send(clientSocket, &pkModify, sizeof(pkModify), 0);
	pthread_cond_wait(&cond, &mutex);
	cout<<"[Modify] Enter new Role: ";
	cin >> Role;
	while(!cin)
    {
            //user didn't input a number
            cin.clear();// reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');//skip bad input
            //newxt, request user reinput
            cout<<"Please enter a number !"<<endl;
            cout<< "Enter the role: ";cin>>Role;
    }
    cin.ignore();
    pkModify.setUP(userNameModify, passWordModify);
	pkModify.setType(modifyUser);
	pkModify.setRole(Role);
    userNameModify.clear();
	passWordModify.clear();
	if(sendPackage(clientSocket,pkModify))
    {
        pthread_cond_wait(&cond, &mutex);
		return;
    }
}

void client::showAllRoles()
{
    package pkModify;
	pkModify.setType(getRoles);
    if(sendPackage(clientSocket,pkModify))
		return;
}

void client::createNewUser()
{
	cout << "\t?ignore -> to exit" << endl;
	do
	{
		char userName[SIZE_U],passWord[SIZE_P];
		string s;
		int role;
		cout<<"\tCreate new user "<<endl;
		//cin.ignore();
		do
		{
			cout << "Enter username: ";
			getline(cin, s);
			if (s.size()>SIZE_U)
			{
				cout << "\tOut size of username ! Please enter again !" << endl;
				continue;

			}
			if(strcmp(strToChar(s),"?ignore")==0)return;
			copy (s.begin(), s.begin()+SIZE_U, userName);
		}
		while (s.size()>SIZE_U);

		do
		{
			cout << "Enter password: ";
			getline(cin, s);
			if (s.size()>SIZE_P)
			{
				cout << "\tOut size of password! Please enter again !"<< endl;
				continue;
			}
			if(strcmp(strToChar(s),"?ignore")==0)return;
			copy (s.begin(), s.begin()+SIZE_P, passWord);
	        //password[SIZE_P+1] = '\0';

		}
		while (s.size()>SIZE_P);
		showAllRoles();
		pthread_cond_wait(&cond, &mutex);
		cout<< "Enter the role: ";
		cin>>role;
		while(!cin)
		{
            //user didn't input a number
            cin.clear();// reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');//skip bad input
            //newxt, request user reinput
            cout<<"Please enter a number !"<<endl;
            cout<< "Enter the role: ";cin>>role;
		}
		cin.ignore();
		package newpk;
		newpk.setType(createUser);
		newpk.setUP(userName,passWord);
		newpk.setRole(role);
		if (sendPackage(clientSocket, newpk))  pthread_cond_wait(&cond, &mutex);
	}while(1);
}


void client::createNewRoom()
{
	string id, cpt;
	int nb;
	package newpk;
	//cin.ignore();
	cout << "\t?ignore -> to exit" << endl;
	while (1)
	{
		cout<<"#! Create New Room !#"<<endl;
		memset(&newpk, 0, sizeof(newpk));
		do
		{
			cout << "Room id: ";		getline(cin, id);
			if (id=="?ignore")
				return;
            if (id.size()>SIZE_U)
			{
				cout << "\tOut size of roomID ! Please enter again !" << endl;
				id.clear();
				continue;

			}
		}while (id.empty());

		do
		{
			cout << "Capacity: ";		getline(cin, cpt);
			if (cpt=="?ignore")
				return;
			nb = atoi(cpt.c_str());
			if (nb<=0)		cout << "invalid capacity!!" << endl;
		}while (nb<=0 || id.empty());

		newpk.setType(createRoom);
		newpk.setRoomM(strToChar(id), nb);
		if (sendPackage(clientSocket, newpk))  pthread_cond_wait(&cond, &mutex);

	}

}

void client::clearUser()
{
	cout<<"\t#! Delete user !#"<<endl;
	//cin.ignore();
	char tempUserName[SIZE_U];
	string s;
	do
	{
		cout << "Enter username: ";
		getline(cin, s);
		if (s.size()>SIZE_U)
		{
			cout << "\tOut size of username ! Please enter again !" << endl;
			continue;

		}
		copy (s.begin(), s.begin()+SIZE_U, tempUserName);
	}
	while (s.size()>SIZE_U);
	package newpk;
	newpk.setType(deleteUser);
	newpk.setUP(tempUserName,(char*)"123");
	if (sendPackage(clientSocket, newpk))
	{
		pthread_cond_wait(&cond, &mutex);
		return;
	}
}

void client::modifyRoomHandler()
{
	int newCapacity;
	package pkRoomModify;
	string roomID_Modify;
	//cin.ignore();
	// insert name of room modify

	do
		{
			cout<<"[Modify] Enter Room Name : ";
            getline(cin, roomID_Modify);
			if (roomID_Modify=="?ignore")
				return;
            if (roomID_Modify.size()>SIZE_U)
			{
				cout << "\tOut size of roomID ! Please enter again !" << endl;
				roomID_Modify.clear();
				continue;

			}
		}while (roomID_Modify.empty());

	cout<<"[Modify] Enter new capacity: ";
	cin >> newCapacity;
	while(!cin)
    {
            //user didn't input a number
            cin.clear();// reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');//skip bad input
            //newxt, request user reinput
            cout<<"Please enter a number !"<<endl;
            cout<<"[Modify] Enter new capacity: ";cin>>newCapacity;
    }
    cin.ignore();
	pkRoomModify.setType(modifyRoom);
	pkRoomModify.setRoomM(strToChar(roomID_Modify), newCapacity);
	sendPackage(clientSocket, pkRoomModify);
	pthread_cond_wait(&cond , &mutex);

}
void client::removeRoom()
{
	cout<<"\t#! Delete Room !#"<<endl;
	//cin.ignore();
	string tempRoomid;

	do
	{

        cout << "Enter Room ID: ";
        getline(cin, tempRoomid);
		if (tempRoomid.size()>SIZE_U)
		{
			cout << "\tOut size of room ID ! Please enter again !" << endl;
			tempRoomid.clear();
			continue;

		}
	}
	while (tempRoomid.size()>SIZE_U);

	package newpk;
	newpk.setRoomM(strToChar(tempRoomid),2 );
	newpk.setType(deleteRoom);
	if (sendPackage(clientSocket, newpk))
	{
		pthread_cond_wait(&cond, &mutex);
		return;
	}
}

void client::getAllUser()
{
	package newpk;
	cout << "\t\t#! Show all user !#" << endl;
	newpk.setType(getuser);
	if (sendPackage(clientSocket, newpk));
	{
		pthread_cond_wait(&cond, &mutex);
		return;
	}
}

void client::findUser(string name)
{
    package newpk;
	newpk.setType(finduser);
	newpk.setMess(strToChar(name));
	if (sendPackage(clientSocket, newpk));
	{
		pthread_cond_wait(&cond, &mutex);
		return;
	}
}
