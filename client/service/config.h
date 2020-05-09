#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <setjmp.h>
#include <iostream>
#include <poll.h>
#include <string>
#include <ctime>
#include <chrono>
using namespace std;

enum color
{
	Red,
	Green,
	Blue,
	Yellow,
	Default
};
#define SIZE_U 20
#define SIZE_P 20
#define SIZE_MESS 4096

#define PORT 54000
#define FAIL -1
#define POLL_SIZE 2

#define FLAG_ON	1
#define FLAG_OFF 0
#define WITH_ON true
#define WITH_OFF false
#define JOIN_ON true
#define JOIN_OFF false
#endif
