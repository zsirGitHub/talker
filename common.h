#ifndef __COMMON_H__
#define __COMMON_H__

#define SERV_PORT 9877
#define CLIENT_MAX 1024 

struct talker_data
{
    int writefd;
    char data[512];
};

#endif
