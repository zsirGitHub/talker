/*
 * =====================================================================================
 *
 *       Filename:  semaphore.h
 *
 *    Description:  semaphore packaging
 *
 *        Version:  1.0
 *        Created:  06/16/2015 10:37:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (zsir), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <sys/types.h>

void create_sem(void);
int set_semvalue(void);
void del_semvalue(void);
int semaphore_p(void);
int semaphore_v(void);

