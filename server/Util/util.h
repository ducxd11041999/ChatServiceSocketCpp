#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;

void SetStdinEcho(bool enable);
char* strToChar(string str);
