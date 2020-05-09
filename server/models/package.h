#include "../config/config.h"
#include "message.h"
#include "commonStruct.h"
union message
{
    struct login Login;

    struct messageSend MessSend;

    struct with With;

    struct join Join;

    struct roomM room;

};


class package
{
private:

    enum eType type;
    union message Message;

public:
    void setMess(char* m)
    {
        strcpy(this->Message.MessSend.mess,m);
        //this->Massage.MessSend.mess = m;
    }
    void setType(eType sType)
    {
        this->type = sType;
    }
    int getType()
    {
        return this->type;
    }
    void setUP(char u[], char p[])
    {
        strcpy(this->Message.Login.uName,u);
        strcpy(this->Message.Login.pWord,p);
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
    void setSender(char *s)
    {
        strcpy(this->Message.MessSend.sender, s);
    }
    void setRoom(char *r)
    {
        strcpy(this->Message.Join.roomName, r);
    }
    void setState(char *s)
    {
        strcpy(this->Message.Login.state, s);
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
    void setOfflineMess(messageSend *msg)
    {
        this->setMess(msg->mess);
        this->setSender(msg->sender);
        this->Message.MessSend.sendTime  = msg->sendTime;
    }
    void setRole(int role)
    {
        Message.Login.role = role;
    } //set role
    int getRole()
    {
        return Message.Login.role;
    }; //get role

//	package()
//	{
//		type = nowork;
//		strcpy(Message.Login.uName,"admin") ;
//		strcpy(Message.Login.pWord,"1") ;
//	}
    ~package()
    {
    }
};


