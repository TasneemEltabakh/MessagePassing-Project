#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#ifndef DISKPROCESS_H
#define DISKPROCESS_H

// for more readability, I JUST but the disk slots and charachters as constants
#define MAX_DATA_LENGTH 64 
#define MAX_DISK_SLOTS 10 


//the 10 diskslots (pointer to them)

//adding and deleting data
int addDataToDisk(const char* data);
int deleteDataFromDisk(char * slot);
void initializeslots();


//it is sent to the kernel to tell it how many slots available
int getStatus();

void SIGUSR2Handler(int signum);
void SIGUSR1Handler(int signum);

void Disk(key_t up, key_t down);
#endif
