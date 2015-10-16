#ifndef __COMMON_H__
#define __COMMON_H__

#define SERV_PORT 9877
#define CLIENT_MAX 512 

struct talker_data
{
    int writefd;
    char data[512];
};

struct talker_info
{
    int connet_flag;
    int talker_fd;
    char talker_name[128];
};

#endif
