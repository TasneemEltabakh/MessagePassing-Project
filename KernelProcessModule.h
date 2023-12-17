#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h> 

void handlersigint();
int up_queue_Disk();
int down_queue_Disk();
int up_queue_User();
int down_queue_User();
void Kernel(key_t diskup, key_t diskdown, key_t userup, key_t userdown);
void clkhandler();
void sendignal1ToDisk();
void sendignal2ToDisk();
void sendignal2ToUser();
void clkupdate();
void SIGUSR2Handler(int signum);
void sendignal2ToMe();