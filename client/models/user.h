#ifndef USER_CLIENT_H
#define USER_CLIENT_H

#define SIZE_U 20
#define SIZE_P 20

using namespace std;


class user
{
private:
    char username[SIZE_U];  // this is username,
    char password[SIZE_P];  // this is password,
    char state[SIZE_U];      // this is room's or user's name
    int roles;              //this is type of user (admin-user)
public:
    volatile bool flag_l = false; //this is used to check login successful of client 

    void setUsername(char uName[]); // set username
    char *getUsername(); // get username


    void setPassword(char pWord[]); // set password
    char *getPassword(); // get password

    void setState(char State[]); // set name
    char *getState(); // get name

    void setroles(int role);
    int getroles();
    user();// contructor empty
};

#endif // USER_H

