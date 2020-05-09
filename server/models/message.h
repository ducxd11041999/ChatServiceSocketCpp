#ifndef MESSAGE_H
#define MESSAGE_H

#include "../config/config.h"

struct messageSend
{
	char mess[SIZE_MESS];
    	char sender[SIZE_U];
	time_t sendTime;
};

#endif//MESSAGE_H
