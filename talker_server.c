/*
 * =====================================================================================
 *
 *       Filename:  talker_server.c
 *
 *    Description:  mini communication system server
 *
 *        Version:  1.0
 *        Created:  06/11/2015 10:21:20 AM
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
    int ret, i, n;
    int listenfd, maxfd, connfd, sockfd;
    fd_set allset, rset;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct talker_info user_list[CLIENT_MAX], user_end;
    char buf[1024];
    struct talker_data *p_talker;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        printf("fail to create socket fd.\r\n");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret < 0) {
        printf("fail to bind fd.\r\n");
        return -1;
    }

    ret = listen(listenfd, CLIENT_MAX);
    if(ret < 0) {
        printf("fail to listen fd.\r\n");
        return -1;
    }

    for(i = 0; i < CLIENT_MAX; i++) {
        user_list[i].connet_flag = 0;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    for( ; ; ) {
        rset = allset;
        ret = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if(ret < 0) {
            printf("select error exit.\r\n");
            return -1;
        }

        if(FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
            if(connfd < 0) {
                printf("fail to accept fd.\r\n");
                return -1;
            }

            printf("connfd:%d \r\n", connfd);
            for(i = 0; i < CLIENT_MAX; i++) {
                if(user_list[i].connet_flag = 0) {
                    user_list[i].connet_flag = 1;
                    user_list[i].talker_fd = connfd;
                    sprintf(user_list[i].talker_name, "user_%d", i);
                    break;
                }
            }
            if(i == CLIENT_MAX) {
                printf("too many users.\r\n");
                return -1;
            }

            FD_SET(connfd, &allset);
            if(connfd > maxfd)
                maxfd = connfd;

            /*return connected user list*/
            for(i = 0; i < CLIENT_MAX; i++) {
                if(user_list[i].connet_flag = 0) {
                    ret = write(connfd, &user_list[i], sizeof(user_list));
                    if(ret < 0) {
                        printf("write user list file\r\n");
                        return -1;
                    }
                    sleep(10);
                }
            }
            
            user_end.connet_flag = 2;
            write(connfd, &user_end, sizeof(user_list));
        }

        for(i = 0; i < CLIENT_MAX; i++) {
            if(user_list[i].connet_flag = 1)
            {
                if(FD_ISSET(user_list[i].talker_fd, &rset)) {
                    if( (n = read(user_list[i].talker_fd, buf, 1024)) > 0 ) {
                        p_talker = (struct talker_data *)buf;
                        write(p_talker->writefd, p_talker->data, 1024);
                        printf("[%d]send msg to [%d]:%s\r\n", sockfd, p_talker->writefd, p_talker->data);
                    }
                }
            }
        }
        
    }
    
    return 0;
}

