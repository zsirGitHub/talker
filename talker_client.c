/*
 * =====================================================================================
 *
 *       Filename:  talker_client.c
 *
 *    Description:  minitalk client
 *
 *        Version:  1.0
 *        Created:  06/11/2015 01:43:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (zsir), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int main(int argc, char **argv)
{
    int ret, n;
    int sockfd;
    fd_set rset, wset;
    struct sockaddr_in servaddr;
    struct talker_data st_talker;
    char buf[1024];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("fail to create socket fd.\r\n");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "192.168.1.115", &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    FD_ZERO(&rset);
    for( ; ; ) {
        FD_SET(sockfd, &rset);
        FD_SET(sockfd, &wset);
        ret = select(sockfd + 1, &rset, &wset, NULL, NULL);
        if(ret < 0) {
            printf("select error exit\r\n");
            return -1;
        }

        if(FD_ISSET(sockfd, &rset)) {
            if(n = read(sockfd, &buf, 1024) > 0 ) {
                printf("client rev: %s\r\n", buf);
            }
        }

        if(FD_ISSET(sockfd, &wset)) {
            fgets(st_talker.data, 512, stdin);
            st_talker.writefd = 4;
            ret = write(sockfd, &st_talker, 1024);
        }
    }


    return 0;
}
