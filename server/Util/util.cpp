#include "util.h"

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
