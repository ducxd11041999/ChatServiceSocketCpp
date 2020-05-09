#include "sock_connect.h"
#include "databaseAccess.h"
#include "../Util/util.h"
#include <unistd.h>
#include <netinet/in.h>

/***********************************************************Funciton init server*************************************/

void sock_connect::Init()
{
    bzero((struct sockaddr *)&server_address,sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(LOCAL_HOST_SOCK);
    int rc = 0;
    int temp = 1;
    int chek;

    while(true)
    {
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        rc = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int));

        if (rc < 0)
        {
            cout << "setsockopt failed!" << strerror(errno) <<endl;
        }

        chek = bind(listenfd, (struct sockaddr *)&server_address, sizeof(server_address));

        if(chek<0)
        {
            cout << "Sorry port is occupied so can't start server,please waitting ! "<< strerror(errno) <<endl;
            sleep(3);
        }
        else
        {
            break;
        }

    }


    listen(listenfd, POLL_SIZE);


    client_fd[0].fd = listenfd;
    client_fd[0].events = POLLRDNORM;

    /*intit client_fd array is emty which value -1*/
    for( i = 1 ; i< POLL_SIZE ; i++)
    {
        client_fd[i].fd = -1;

    }

    mapUser(this->mapuser);
    mapRoom(this->roomMap);
    maxi = 0;
    cout<<"Waiting for client......... \n"<<listenfd<<endl;


}

/***********************************************************Funciton init array*************************************/

void sock_connect::EventServer()
{
    if(client_fd[0].revents & POLLRDNORM) /*New client connection*/
    {
        clilen = sizeof(client_address);
        connfd = accept(listenfd, (struct sockaddr *)&client_address, &clilen);

        for(i =1 ; i< POLL_SIZE; i++)
        {
            if(client_fd[i].fd<0)
            {
                cout<<"Socket have fd: "<< connfd <<" is connected"<<endl;
                client_fd[i].fd = connfd;/*Save descriptor*/
                break;
            }
        }

        if(i == POLL_SIZE)
        {
            strcpy(buf,"too many client");/*Max index in client[] array*/
            write(connfd,buf,MAXLINE);
        }

        client_fd[i].events = POLLRDNORM;

        if(i>maxi)
            maxi = i;

        if(--nready <=0 )
            return; /*No more readable descriptors*/
    }

}

void sock_connect::EventClient()
{




}

/***********************************************************Funciton main of server*************************************/

void sock_connect::Execute()
{

    while(true)
    {

        nready = poll(client_fd, maxi +1, -1);
        EventServer();

        for( i = 1; i<= maxi ; i++) /**Check all client for data*/
        {
            package pk;

            if((sockfd = client_fd[i].fd) < 0)
                continue;

            if( client_fd[i].revents & (POLLRDNORM | POLLERR))
            {
                if ( (n = read(sockfd, &pk, sizeof(package))) <0)
                {
                    if( errno == ECONNRESET)
                    {
                        /**connection rest by client*/
                        if(checkfd(sockfd))
                        {
                            notificationOffline(sockfd,offlineuser);
                            if(changeStateUserOff(sockfd))
                            {
                                cout<< "Change state successful !"<<endl;

                            }
                            else
                            {
                                cout <<"error change state !"<<endl;

                            }
                        }
                        close(sockfd);
                        client_fd[i].fd =-1;
                    }
                    else
                    {
                        cerr<<"read eror"<<endl;
                    }
                }
                else if (n==0)
                {
                    /**connection close by client*/
                    cout<<"Socket have fd : "<< sockfd <<"is closing"<<endl;
                    if(checkfd(sockfd))
                    {
                        notificationOffline(sockfd,offlineuser);
                        cout<<sockfd<<" fd change state"<<endl;
                        if(changeStateUserOff(sockfd))
                        {
                            cout<< "Change state successful !"<<endl;

                        }
                        else
                        {
                            cout <<"error change state !"<<endl;

                        }
                    }
                    close(sockfd);
                    client_fd[i].fd = -1;
                }
                else
                {
                    analyzePackage(sockfd,&pk);
                }

                if (--nready <= 0)
                    break; /** no more readable descriptors */
            }
        }
    }

    close(client_fd[0].fd);
}


/***********************************************************Funciton handle package from client*************************************/
void sock_connect::analyzePackage(int client,package*pk)
{
    cout <<"server nhan: "<<pk->getType()<<endl;
// i need to check fd send package has login success, don't me?
    if(!checkfd(client) && pk->getType()!= loginn){
        pk->setMess((char*)"Please login to use function!");
        pk->setType(sendFail);
        write(client,&pk,sizeof(package));
        return;
    }
//
    switch (pk->getType())
    {
    case loginn:
        Login(client,pk);
        break;

    case withh:
        WithFunction(client,pk);
        break;

    case joinn:
        JoinRoom(client, pk);
        break;

    case sendMess:
        OneToOneChat(client,pk);
        break;

    case terminateChat:
        LeaveChatUser(client,pk);
        break;

    case showRoom:
        ShowRoom(client);
        break;

    case getRoles:
        getListRoles(client,pk);
        break;

    case createUser:
        AddUser(client, pk);
        break;

    case modifyUser:
        handleModifyUser(client, pk);
        break;
    case deleteUser:
        DeleteUser(client, pk);
        break;

    case modifyRoom:
        ModifyRoom(client, pk);
        break;

    case deleteRoom:
        DeleteRoom(client, pk);
        break;
    case createRoom:
        addRoom(client, pk);
        break;
    case getuser:
        getUserOnline(client,pk);
        break;

    case finduser:
        FindUser(client, pk);
        break;
    }

}

void sock_connect::AddUser(int client, package *pk)
{
    package newPkg;
    user us;// new user
    message m = pk->showAction();
    us.setUsername(m.Login.uName);

    map<user, int>::iterator it;
    it = mapuser.find(us);

    if (!CheckAdmin(client))
    {
        newPkg.setType (createFail);
        newPkg.setMess ((char *)"You do not have permission call this function");
    }
    else if (it!=mapuser.end())
    {
        // isfound
        newPkg.setType (createFail);
        newPkg.setMess ((char *)"You can't create this user because it is already");
    }
    else
    {
        if (strcmp(m.Login.uName, "")==0 || strcmp(m.Login.pWord, "")==0)
        {
            newPkg.setType (createFail);
            newPkg.setMess ((char*)"Sorry. User name or pass word is null");
        }
        else
        {
            newPkg.setType (createSuccess);
            newPkg.setMess ((char *)"Create success");
            us = user(m.Login.uName, m.Login.pWord);
            us.setRole(m.Login.role);
            if(us.getRole()!= 1){
             us.setRole(2);
            }else us.setRole(1);
            insertUser(us);
            mapuser.insert (pair<user, int>(us, -1));
        }
    }
    write (client, &newPkg, sizeof (newPkg));

}
/***********************************************************Funciton handle deleted user for admin*************************************/

void sock_connect::DeleteUser(int client_fd, package *pk)
{
    package newPkg;
    user us;// new user
    message m = pk->showAction();

    us.setUsername(m.Login.uName);
    map<user, int>::iterator it;
    it = this->mapuser.find(us);
    if (!CheckAdmin(client_fd))
    {
        newPkg.setType (deleteFail);
        newPkg.setMess ((char *)"You do not have permission call this function");
    }
    else if (it==mapuser.end())
    {
        // isfound
        newPkg.setType (deleteFail);
        newPkg.setMess ((char *)"Sorry username does not exit !");
    }
    else if(it->second>0)
    {
        newPkg.setType (deleteFail);
        newPkg.setMess ((char *)"Sorry user is online. You can't delete this user !");
    }
    else if(us.getRole()==1)
    {
        newPkg.setType (deleteFail);
        newPkg.setMess ((char *)"Sorry you can't delete this user because it is admin account !");
    }
    else if(haveOfflineMessage(us))
    {
        newPkg.setType (deleteFail);
        newPkg.setMess ((char *)"Sorry you can't delete this user because it has offline message !");
    }
    else
    {
        us = it->first;
        newPkg.setType (deleteSuccess);
        deleteUserDB(us);
        mapuser.erase(us);
        newPkg.setMess ((char *)"Deleted successfully !");
    }
    write (client_fd, &newPkg, sizeof (newPkg));
}

void sock_connect::handleModifyUser(int client, package *pk)
{
    package newPkg;
    user us;
    message m = pk->showAction();
    us.setUsername(m.Login.uName);
    us.setPassword(m.Login.pWord);
    us.setRole(m.Login.role);
    cout<<m.Login.uName<<" username modify"<<endl;
    map<user, int>::iterator it;
    it = mapuser.find(us);

    if (!CheckAdmin(client))
    {
        newPkg.setType (modifyFail);
        newPkg.setMess ((char *)"You do not have permission call this function");
    }
    else
    {
        if (it != mapuser.end()) // user is found
        {
            string newPassword = us.getPassword();

            if ( newPassword.compare("") == 0 ) // Entered password is empty
            {
                newPkg.setType (modifyFail);
                newPkg.setMess ((char *)"Sorry, can't modify user because password is empty");
            }
            else
            {
                int userFd = getfdUsername(us.getUsername());
                if (userFd > 0) // user is online
                {
                    newPkg.setType (modifyFail);
                    newPkg.setMess ((char *)"User is online, You can not modify the information");
                }
                else    // All conditions are OK
                {
                    // Change password in map
                    user modifiedUser = it -> first;
                    modifiedUser.setPassword(newPassword);
                    if(us.getRole()!= 1){
                        modifiedUser.setRole(2);
                    }else modifiedUser.setRole(1);
                    mapuser.erase(it->first);
                    this->mapuser.insert(pair<user,int> (modifiedUser,userFd));
                    // Save to database
                    modifyUserDB(modifiedUser);
                    // Set data for Package
                    newPkg.setMess ((char *)"Modify successfully");
                    newPkg.setType (modifySuccess);
                }
            }
        }
        else                    // user is not found
        {
            newPkg.setType (modifyFail);
            newPkg.setMess ((char *)"Username is not exist");
        }
    }
    write (client, &newPkg, sizeof (newPkg));
}

/***********************************************************Funciton handle required connect one-to-one chat by client*************************************/
void sock_connect::WithFunction(int client_fd,package *pk)
{

    user temp;
    message m = pk->showAction();

    temp.setUsername(m.With.userRev);
    user result;
    user Sender;
    map<user, int>::iterator it;
    it =mapuser.find(temp);

    if(it != mapuser.end()) // if the user is found;
    {
        int connection = it->second;

        if(connection == client_fd)
        {
            package newpk;
            newpk.setType(withFail);
            newpk.setMess((char*)"You cannot chat with yourself!");
            write (client_fd,&newpk,sizeof(package));
            return;

        }

        result = it -> first;

        if(connection < 0)
        {
           map<user,int>::iterator indexer;

            for (indexer = mapuser.begin(); indexer!= mapuser.end(); indexer++)
            {

                if(indexer->second == -1)
                {
                    continue;
                }

                if(indexer->second == client_fd)
                {
                    Sender = indexer -> first;
                    Sender.setState(1,temp.getUsername());
                    mapuser.erase(indexer->first);
                    this->mapuser.insert(pair<user,int> (Sender,client_fd));
                    updateState(Sender);
                    package newpk;
                    newpk.setType(withSuccess);
                    newpk.setTypeWith(offlineuser);
                    write (client_fd,&newpk,sizeof(package));
                    break;

                }

            }
        }
        else
        {
            map<user,int>::iterator indexer;

            for (indexer = mapuser.begin(); indexer!= mapuser.end(); indexer++)
            {

                if(indexer->second == -1)
                {
                    continue;
                }

                if(indexer->second == client_fd)
                {
                    Sender = indexer -> first;
                    Sender.setState(1,temp.getUsername());
                    mapuser.erase(indexer->first);
                    this->mapuser.insert(pair<user,int> (Sender,client_fd));
                    updateState(Sender);
                    package newpk;
                    newpk.setType(withSuccess);
                    newpk.setTypeWith(onlineuser);
                    write (client_fd,&newpk,sizeof(package));
                    break;

                }

            }

        }

        return;
    }

    package newpk;
    newpk.setType(withFail);
    newpk.setMess((char*)"This user is not found");
    write (client_fd,&newpk,sizeof(package));
    return;

}

/***********************************************************Funciton handle send package for chat one-to-one*************************************/
void sock_connect::OneToOneChat(int client_fd,package*pk)
{
    map<user, int>::iterator it;

    user Sender;

    for (it = mapuser.begin(); it != mapuser.end(); it ++)
    {
        if(it->second == -1)
        {
            continue;
        }

        if(it->second == client_fd)
        {
            Sender = it-> first;

        }
    }

    if(Sender.getState().type == 1) // check if the user is with another user
    {
        user temp; // Create a temp user
        temp.setUsername( Sender.getState().name); // set the temp user's name to receiver name for finding the key in
        // map


        map<user, int>::iterator receiver;
        receiver = mapuser.find(temp); // find the key user based on username


        if(receiver != mapuser.end()) // if the user is found;
        {
            int connection = receiver -> second;

            if(connection < 0)  // if the user is not online
            {
                cout<< "client da offline va can luu vao csdl !fd: "<<client_fd<<endl;
                //Handle if the receiver is offline, store the message in the database
                pk->setSender(strToChar(Sender.getUsername()));
                message msg = pk->showAction();
                saveMessage(msg.MessSend, Sender.getState().name);

            }
            else // if the user is online
            {
                //cout<< "send for fd: "<<sockfd<<endl;
                pk->setSender(strToChar(Sender.getUsername()));
                write(connection,pk,sizeof(package));
            }
        }
    }

    else// if the user joined a room
    {
        if(Sender.getState().type == 2)
        {
          RoomChat(client_fd,pk,Sender);
        }
    }


}



/***********************************************************Funciton handle leave chat by client*************************************/

void sock_connect::LeaveChatUser(int client_fd, package*pk)
{
    user Sender = findUserByfd(client_fd);  // Find the user through its fd

    if(Sender.getState().name == "") // check if the sender is connected with anyone
    {
        package newpk;// Create a package to send to the sender
        char* message = (char*)"You are not connected with anyone."; // Create a message
        newpk.setType(failTerminate);
        newpk.setMess(strToChar(message));
        write(client_fd,&newpk,sizeof(package)); // Send the package to user
        return;
    }

    string receiver_name =  Sender.getState().name; // Find the receiver's username through the state in sender
    string message = "" + receiver_name; // Create a message

    if(Sender.getState().type == 1)
    {
        message = "You disconnected with the user: " + receiver_name +"."; // Create a message
    }
    else if (Sender.getState().type == 2)
    {

        message = "You left the room: " + receiver_name +"."; // Create a message
        LeaveRoom(client_fd,Sender);
    }



    map< user,int>::iterator temp;

    for (temp = this->mapuser.begin(); temp != this->mapuser.end(); ++temp)
    {
        if(temp->second == client_fd)
        {
            user userdis  = temp-> first;
            clearState(temp->first);
            mapuser.erase(temp->first);
            userdis.setState(0,"");
            this->mapuser.insert(pair<user,int> (userdis,client_fd));
            break;
        }
    }

    package newpk;// Create a package to send to the sender
    newpk.setType(successTerminate);
    newpk.setMess(strToChar(message));
    write(client_fd,&newpk,sizeof(package)); // Send the package to user


}
/***********************************************************Funciton to find user by server*************************************/
void sock_connect::FindUser(int client_fd,package*pk)
{
    message m = pk->showAction();
    string target(m.MessSend.mess) ; // get a taget's name
    string message = "["; //create a message string
    package newpk; // create a new package for sending purpose
    map<user,int>::iterator it; // create iterator for finding purpose
    if(target == "*") // if the content = *, show all the users
    {
        int counter = 0;
        for(it = mapuser.begin(); it != mapuser.end(); it ++)
        {
            counter++;
            user temp = it -> first;
            string username = temp.getUsername();
            int status = it -> second;
            if(status > 0) // if the user is online, add* to the name
            {
                message += username + "*";
            }
            else
            {
                message += username;
            }
            if (counter != (int)mapuser.size())
                message += " | ";
        }
        message += "]";

        newpk.setType(showUsers); // set package type
        newpk.setMess(strToChar(message)); // set package mess
        write(client_fd,&newpk,sizeof(package)); // send the package
        return;
    }

    /* use function from repo to find username with certain char. */
    user *targets = findUserWithName(target);
    int length = countUserWithName(target);
    if (length==0)
    {
        message = "No users found!";
    }
    else
    {
        int counter = 0;
        for (int n = 0; n < length; n ++)
        {
            it = mapuser.find(targets[n]);
            if(it!= mapuser.end())
            {
                counter++;
                user temp = it -> first;
                string username = temp.getUsername();
                int status = it -> second;
                if(status > 0)
                {

                    message += username + "*";
                }
                else
                {
                    message += username;
                }
                if (counter!=length)
                    message += " | ";
            }
        }
        message += "]";
    }

    newpk.setType(showUsers); // set package type
    newpk.setMess(strToChar(message)); // set package mess
    write(client_fd,&newpk,sizeof(package)); // send the package
    return;
}
/***********************************************************Funciton to check admin access by server*************************************/
bool sock_connect::CheckAdmin(int fd)
{
    user u = findUserByfd(fd);

    if(u.getUsername()!= "") // check if the user is found
    {
        if(u.getRole() == 1) //check if the role is admin
        {
            return true;
        }
    }

    return false;
}
/***********************************************************Funciton to add room (admin) by server*************************************/
void sock_connect::addRoom(int client_fd,package*pk)
{
    package newpk;  // create a new package to send back to the user

    if(CheckAdmin(client_fd)) // check if the user is admin
    {
        message Mess = pk->showAction(); // get the message struct in the package

        map <string, Room>:: iterator it = roomMap.find(Mess.room.roomID); // create the iterator and find it through string roomID

        if(it == roomMap.end()) // if the room already existed
        {
            if(Mess.room.capacity >= 0&& string(Mess.room.roomID)!="") // the roomID is not empty + capacity is valid
            {
                Room new_room (Mess.room.roomID,Mess.room.capacity); // create new room;
                insertRoom(new_room); // insert room to room table in my sql database
                roomMap.insert(pair<string,Room>(Mess.room.roomID,new_room)); // insert the new room into the map

                newpk.setType(createSuccess); // set the type
                newpk.setMess((char*)"You have created the room."); // set the message
                write(client_fd,&newpk,sizeof(package)); // send the package
                return;

            }
            //if room ID is empty + capacity is not valid
            newpk.setType(createFail); // set the type
            newpk.setMess((char*)"The roomID and capacity cannot be empty,"); // set the message
            write(client_fd, &newpk, sizeof(package)); // send the package to the user
            return;

        }

        newpk.setType(createFail); // set the type
        newpk.setMess((char*)"The roomID already existed!"); // set the message
        write(client_fd, &newpk, sizeof(package)); // send the package to the user
        return;


    }

    // if the user is not admin
    newpk.setType(createFail); // set the type
    newpk.setMess((char*)"You dont have permission to call this function"); // set the message
    write(client_fd, &newpk, sizeof(package)); // send the package to the user
}
/***********************************************************Funciton handle join room by server*************************************/

void sock_connect::JoinRoom(int client_fd,package*pk)
{
    char* roomID = pk->showAction().Join.roomName;
    map< user,int>::iterator temp;
    package newpk;

    map< string,Room>::iterator indexer;
    string x(roomID);
    indexer = roomMap.find(x);

    if(indexer != roomMap.end()) // find the room
    {
        Room room = (Room)indexer->second;

        if(room.add(client_fd) < 0)
        {
            newpk.setMess((char*)"The room reaches its capacity or you are already in the room.");
            newpk.setType(joinFail);
            write(client_fd,&newpk,sizeof(package));
            return;
        }else{
            indexer->second = room;
        }

        //changer user state
        // reseach name user and change state
        for (temp = this->mapuser.begin(); temp != this->mapuser.end(); ++temp)
        {
            if(temp->second == client_fd)
            {
                user userdis  = temp-> first;
                clearState(temp->first);
                mapuser.erase(temp->first);
                userdis.setState(2,x);
                this->mapuser.insert(pair<user,int> (userdis,client_fd));
                updateState(userdis);
                break;
            }
        }

        string message = "You entered room <" + x +">";
        newpk.setMess(strToChar(message));
        newpk.setType(joinSuccess);
        write(client_fd,&newpk,sizeof(package));
        indexer -> second = room;
        return;

    }

    newpk.setMess((char*)"The room does not exist.");
    newpk.setType(joinFail);
    write(client_fd,&newpk,sizeof(package));
    return;
}

/***********************************************************Funciton do room chats*************************************/
void sock_connect::RoomChat(int client_fd,package*pk,user u)
{
    char*roomID = strToChar(u.getState().name);
    package newpk;
    Room room;
    map< string,Room>::iterator indexer;
    string x(roomID);
    indexer = roomMap.find(x);
    if(indexer != roomMap.end()) // find the room
    {
        room = indexer -> second;
    }

    int *fdRoomMembers = room.listClients();
    for( int i = 0 ; i < room.getOnlUser(); i ++)
    {
        pk->setSender(strToChar(u.getUsername()));
        write(fdRoomMembers[i],pk,sizeof(package));
    }

}
/***********************************************************Funciton leave room for client*************************************/
void sock_connect::LeaveRoom(int client_fd,user u)
{
    string roomName = u.getState().name;
    map <string,Room>::iterator indexer;
    indexer = roomMap.find(roomName);

    if(indexer != roomMap.end())
    {
        Room temp = indexer -> second;
        temp.removeUser(client_fd);
        int availableUsers = temp.getOnlUser();
        if(availableUsers>0)
        {
            package newpk;
            char* mess = strToChar("");
            char sender[SIZE_U];
            strcpy(mess, (char*)"Notification: ");
            strcat(mess,strToChar(u.getUsername()));
            strcat(mess,(char*)" left room.");
            strcpy(sender,(char*)"System");
            newpk.setTime();
            newpk.setMess(mess);
            newpk.setSender(sender);
            newpk.setType(sendMess);
            int *fdRoomMembers = temp.listClients();

            for( int i = 0 ; i < temp.getOnlUser(); i ++)
            {
                write(fdRoomMembers[i],&newpk,sizeof(package));
            }

            //indexer -> second  = temp;
        }
 	roomMap.erase(indexer->first);
        roomMap.insert(pair<string,Room>(temp.getRoomID(),temp));
        return;
    }
}

/***********************************************************Funciton get rooms send for client*************************************/
void sock_connect::ShowRoom(int client_fd)
{
    string listshow = "[";
    map< string,Room>::iterator indexer;
    map< string,Room>::iterator indexertemp;

    package newpk;

    for(indexer = roomMap.begin(); indexer != roomMap.end(); indexer ++)
    {
        indexertemp=indexer;
        string line = "RoomID: " +string(indexer -> second.getRoomID()) +
                      " | Capacity: " + to_string(indexer ->second.getOnlUser()) +
                      "/" +to_string(indexer ->second.getCapacity()) ;

        listshow += line;

        if(++indexertemp != roomMap.end())
        {
            listshow += "\n ";
        }
    }

    listshow += "]";
    newpk.setMess(strToChar(listshow));
    newpk.setType(allRoom);
    //message s = newpk.showAction();
    write (client_fd,&newpk,sizeof(package));
}




/***********************************************************Funciton find user by fd*************************************/
user sock_connect::findUserByfd(int fd)
{
    map< user,int>::iterator temp;

    for (temp = this->mapuser.begin(); temp != this->mapuser.end(); ++temp)
    {
        if(temp->second == fd)
        {
            return temp->first;
        }
    }

    user u;
    return u;
}

/***********************************************************Funciton handle state client when client disconnect*************************************/

bool sock_connect::changeStateUserOff(int fd)
{

    map< user,int>::iterator temp;
    for (temp = this->mapuser.begin(); temp != this->mapuser.end(); ++temp)
    {
        if(temp->second == fd)
        {
            user userdis  = temp-> first;

            if(userdis.getState().type == 2)
            {
                LeaveRoom(fd,userdis);
            }

            clearState(userdis);
            mapuser.erase(temp->first);
            userdis.setState(0,"");
            this->mapuser.insert(pair<user,int> (userdis,-1));
            return true;
        }
    }

    return false;
}


/*********************************************************Funciton get fd of client by username*************************************/

int sock_connect::getfdUsername(string username)
{
    map< user,int>::iterator temp;
    user u ;
    u.setUsername(username);
    temp = mapuser.find(u);

    if(temp != mapuser.end())
    {
        return temp-> second;
    }

    return -1;
}


/***********************************************************Funciton handle login from client*************************************/

void sock_connect::Login(int client_fd,package *pk)
{
    package pakSen;
    map< user,int>::iterator temp;
    message m = pk->showAction();
    user u ;
    u.setUsername(m.Login.uName);
    temp = mapuser.find(u);

    if(temp != mapuser.end())
    {
        u= temp->first;

        if(temp->second > 0)
        {
            pakSen.setType(loginFail);
            pakSen.setMess((char*)"Sorry your account is being login elsewhere !!");

            write(client_fd,&pakSen,sizeof(package));
            return;
        }

        if(strcmp(strToChar(u.getPassword()),m.Login.pWord)==0)
        {
            pakSen.setType(loginSuccess);
            pakSen.setMess((char*)"You have succcessfully logged in!");
            pakSen.setRole(u.getRole());
            user usr = temp->first;
            pakSen.setUP((char *)strToChar(usr.getUsername()), (char*)strToChar(usr.getPassword()));
            pakSen.setState((char *)strToChar(usr.getState().name));
            temp->second = client_fd;
            if(usr.getState().type == 2)
            {
                map< string,Room>::iterator indexer;
                indexer = roomMap.find(usr.getState().name);

                if(indexer != roomMap.end()) // find the room
                {
                    Room room = (Room)indexer->second;

                    if(room.add(client_fd) < 0)
                    {
                        cout<<"bug add room"<<endl;
                        return;
                    }else{

                        indexer->second = room;

                    }
                }
            }

            write(client_fd,&pakSen,sizeof(package));
            handleOfflineMessage(client_fd);
            notificationOffline(client_fd,onlineuser);
        }
        else
        {

            pakSen.setType(loginFail);
            pakSen.setMess((char*)"Wrong pass !!");
            write(client_fd,&pakSen,sizeof(package));
        }


    }
    else
    {
        pakSen.setType(loginFail);
        pakSen.setMess((char*)"Username not exist!!");
        write(client_fd,&pakSen,sizeof(package));

    }
}

void sock_connect::ModifyRoom(int client_fd, package *pk)
{
    user usr = findUserByfd(client_fd);
    package newpk;
    string mess;
    if (usr.getRole() == 1)
    {
        message m = pk->showAction();
        map<string, Room>::iterator it;
        it = roomMap.find(m.room.roomID);

        if (it != roomMap.end())
        {
            Room room = it->second;

            if (m.room.capacity>0)
            {
                if (room.getOnlUser()==0)
                {
                    room.setRoomID(m.room.roomID);
                    room.setCapacity(m.room.capacity);

                    it->second = room;
                    modifyRoomDB(room);
                    newpk.setType(modifySuccess);
                    mess = "Successfully modified room '" + string(m.room.roomID) + "'!";
                    newpk.setMess(strToChar(mess));
                    write(client_fd,&newpk,sizeof(package));
                    return;
                }
                else
                {
                    newpk.setType(modifyFail);
                    mess = "There are users currently in room '" + string(m.room.roomID) + "'!";
                    newpk.setMess(strToChar(mess));
                    write(client_fd,&newpk,sizeof(package));
                    return;
                }
            }
            else
            {
                newpk.setType(modifyFail);
                mess = "RoomID or capacity is invalid!";
                newpk.setMess(strToChar(mess));
                write(client_fd,&newpk,sizeof(package));
                return;
            }
        }
        else
        {
            newpk.setType(modifyFail);
            mess = "Room '" + string(m.room.roomID) + "' does not exists!";
            newpk.setMess(strToChar(mess));
            write(client_fd,&newpk,sizeof(package));
            return;
        }

    }
    else
    {
        newpk.setType(modifyFail);
        mess = "Permission denied!";
        newpk.setMess(strToChar(mess));
        write(client_fd,&newpk,sizeof(package));
        return;
    }

    return;

}

void sock_connect::DeleteRoom(int client_fd, package *pk)
{
    user usr = findUserByfd(client_fd);
    package newpk;
    string mess;
    if (usr.getRole() == 1)
    {
        message m = pk->showAction();
        map<string, Room>::iterator it;
        it = roomMap.find(m.room.roomID);

        if (it != roomMap.end())
        {
            Room room = it->second;

            if (room.getOnlUser()==0)
            {
                roomMap.erase(it);
                deleteRoomDB(room);
                newpk.setType(deleteSuccess);
                mess = "Successfully deleted room '" + string(m.room.roomID) + "'!";
                newpk.setMess(strToChar(mess));
                write(client_fd,&newpk,sizeof(package));
                return;
            }
            else
            {
                newpk.setType(deleteFail);
                mess = "There are users currently in room '" + string(m.room.roomID) + "'!";
                newpk.setMess(strToChar(mess));
                write(client_fd,&newpk,sizeof(package));
                return;
            }

        }
        else
        {
            newpk.setType(deleteFail);
            mess = "Room '" + string(m.room.roomID) + "' does not exist!";
            newpk.setMess(strToChar(mess));
            write(client_fd,&newpk,sizeof(package));
            return;
        }

    }
    else
    {
        newpk.setType(deleteFail);
        mess = "Permission denied!";
        newpk.setMess(strToChar(mess));
        write(client_fd,&newpk,sizeof(package));
        return;
    }

    return;
}

void sock_connect::handleOfflineMessage(int client_fd)
{
    user usr = findUserByfd(client_fd);
    package pk;
    messageSend *messageArr = getMessageOfUser(usr);

    if (messageArr == nullptr)
        return;

    while (strlen(messageArr->mess) > 0)
    {
        pk.setOfflineMess(messageArr);
        pk.setType(sendMess);
        cout << pk.getLenMess()  << endl;
        write(client_fd,&pk,sizeof(package));
        messageArr++;
    }

    deleteMessageOfUser(usr);
}

/***********************************************************Funciton get all roles from database and send for client*************************************/

void sock_connect::getListRoles(int client_fd,package *pk)
{
    package pksen;
    pksen.setType(showRoles);
    pksen.setMess(strToChar(getAllRoles()));
    cout<<getAllRoles()<<endl;
    write(client_fd,&pksen,sizeof(package));
}

////////////////////// checkfd ///////////////////////////////
bool sock_connect::checkfd(int fd)
{
    map< user,int>::iterator temp;

    for (temp = this->mapuser.begin(); temp != this->mapuser.end(); ++temp)
    {
        if(temp->second == fd)
        {
            return true;
        }
    }

    return false;
}

void sock_connect::getUserOnline(int client_fd,package *pk)
{
    bool flag_first =true;
    string listshow = "[";
    map< user,int>::iterator indexer;
    package newpk;
    for(indexer = mapuser.begin(); indexer != mapuser.end(); indexer ++)
    {
        if(indexer->second>0)
        {
            user tempUser = indexer -> first;

            if(flag_first)

            {
                listshow += tempUser.getUsername();
                flag_first =false;
            }
            else
            {
                listshow += "|";
                listshow += tempUser.getUsername();

            }
        }

    }
    listshow += "]";
    newpk.setType(showUsers);
    newpk.setMess(strToChar(listshow));
    write (client_fd,&newpk,sizeof(package));
}


/***********************************************************Funciton get all roles from database and send for client*************************************/

void sock_connect::notificationOffline(int client_fd,eType e)
{
    map< user,int>::iterator indexer;
    user usr = findUserByfd(client_fd);
     cout << "vao "<<usr.getUsername()<<endl;
    package newpk;
    newpk.setType(checkparty);
    newpk.setTypeWith(e);
      if(usr.getUsername() != ""){
        for(indexer = mapuser.begin(); indexer != mapuser.end(); indexer ++)
        {
           user tempUser = indexer -> first;
            if(indexer->second>0&&tempUser.getState().type == 1 && tempUser.getState().name == usr.getUsername() )
            {
                write (indexer->second,&newpk,sizeof(package));
                cout<<"send for: "<< indexer->second<<endl;
            }
        }

      }
}

