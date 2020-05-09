#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;

void SetStdinEcho(bool enable)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if ( !enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDERR_FILENO, TCSANOW, &tty);
}

char* strToChar(string str)
{
    int size = str.length();
    char *cStr = new char[size+1];
    strcpy(cStr, str.c_str());
    return cStr;
}

string timeToStr(time_t &time)
{
    string sTime = ctime(&time);
    return sTime.substr(0, sTime.length()-1);
}
