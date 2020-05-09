#include "../service/config.h"





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

struct dataLogin
{
	char *u;
	char *p;
};

union message
{
	struct login
	{
		char uName[SIZE_U];
		char pWord[SIZE_P];
		char state[SIZE_U]; // ONLY FOR RECEIVING LAST STATE WHEN RECONNECTING
		int role;
	}Login;

	struct messageSend
	{
		char mess[SIZE_MESS];
		char sender[SIZE_U];
		time_t sendTime;
		//int len;
		// need to initilize few option
	}MessSend;

	struct with
	{
		char userRev[SIZE_U];
		eType userStatus;

	}With;

	struct join
	{
		char roomName[SIZE_U];

	}Join;

	//SUPPORT FOR ROOM FUNCTION
	struct roomM
    {
        char roomID[SIZE_U];
        int capacity;
    }room;

};

class package
{
	private:

	enum eType type;
	union message Message;

	public:
	void setMess(char* m)
	{
		// get system  time
		setTime();
		// set message
		strcpy(this->Message.MessSend.mess,m);
	}
	void setType(eType sType)
	{
		this->type = sType;
	}
	int getType(){
		return this->type;
	}
	void setUP(char u[], char p[])
	{
		strcpy(this->Message.Login.uName, u);
		strcpy(this->Message.Login.pWord, p);
	}
	void setUP(string u, string p)
	{
		strcpy(this->Message.Login.uName, u.c_str());
		strcpy(this->Message.Login.pWord, p.c_str());
	}
	int getLenMess()
	{
		return strlen(Message.MessSend.mess);
	}
	void showType()
	{
		cout<<this->type<<endl;
	}
	void setRev(char *u)
	{
		strcpy(this->Message.With.userRev, u);

	}
	void setRoom(char *r)
	{
		strcpy(this->Message.Join.roomName, r);
	}
	void setSender(char *s)
	{
        strcpy(this->Message.MessSend.sender, s);
	}

	void setRoomM(char *roomID, int capacity)
    {
        strcpy(this->Message.room.roomID, roomID);
        this->Message.room.capacity = capacity;
    }
    // user status online or offline of with
    void setTypeWith(eType e)
    {
        this->Message.With.userStatus = e;
    }

	union message showAction()
	{
			return this->Message;
	}
	void setTime()
	{
		auto tempTime = std::chrono::system_clock::now();
		this->Message.MessSend.sendTime = chrono::system_clock::to_time_t(tempTime);
	}
	void setRole(int role)
    {
        Message.Login.role = role;
    } //set role
    int getRole()
    {
        return Message.Login.role;
    }; //get role
	package()
	{
		type = nowork;
		strcpy(Message.Login.uName,"admin") ;
		strcpy(Message.Login.pWord,"1") ;
	}
	~package()
	{
	}
};


