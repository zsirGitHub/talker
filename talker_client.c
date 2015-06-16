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
#include <pthread.h>
#include <sys/sem.h>
#include "common.h"
#include "semaphore.h"

static void talker_read_task(void *user_param);
static void talker_write_task(void *user_param);

int main(int argc, char **argv)
{
    int ret, n;
    int sockfd;
    pthread_t tid;
    pthread_t tid_1;
    struct sockaddr_in servaddr;
    
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

    //create_sem();
    //set_semvalue(); /*not need semaphore sync*/
    pthread_create(&tid, NULL, (void *)talker_read_task, (void *)&sockfd);
    pthread_create(&tid_1, NULL, (void *)talker_write_task, (void *)&sockfd);

    while(1) {
        sleep(1);
    }

    return 0;
}


static void talker_read_task(void *user_param)
{
    int ret, n;
    int sockfd;
    fd_set rset;
    char buf[1024];
    struct talker_data st_talker;

    sockfd = (int)(*(int *)user_param);
    FD_ZERO(&rset);
    for( ; ; ) {
        FD_SET(sockfd, &rset);
        ret = select(sockfd + 1, &rset, NULL, NULL, NULL);
        if(ret < 0) {
            printf("select error exit\r\n");
            return;
        }

        //semaphore_p();
        if(FD_ISSET(sockfd, &rset)) {
            if(n = read(sockfd, &buf, 1024) > 0 ) {
                printf("she says: %s\r\n", buf);
            }
        }
        //semaphore_v();
    }
}

static void talker_write_task(void *user_param)
{
    int ret;
    int sockfd;
    fd_set wset;
    char buf[1024];
    struct talker_data st_talker;
    
    sockfd = (int)(*(int *)user_param);
    FD_ZERO(&wset);
    for( ; ; ) {
        FD_SET(sockfd, &wset);
        ret = select(sockfd + 1, NULL, &wset, NULL, NULL);
        if(ret < 0) {
            printf("select error exit\r\n");
            return;
        }

        //semaphore_p();
        if(FD_ISSET(sockfd, &wset)) {
            fgets(st_talker.data, 512, stdin);
            st_talker.writefd = 4;
            ret = write(sockfd, &st_talker, 1024);
        }
        //semaphore_v();
    }
}
