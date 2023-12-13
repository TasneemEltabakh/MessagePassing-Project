#include <stdbool.h>
#include <signal.h>
#ifndef DISKPROCESS_H
#define DISKPROCESS_H

// for more readability, I JUST but the disk slots and charachters as constants
#define MAX_DATA_LENGTH 64 
#define MAX_DISK_SLOTS 10 


//the 10 diskslots (pointer to them)

//adding and deleting data
void addDataToDisk(const char* data);
void deleteDataFromDisk(int slotIndex);

//just to be busy while deleting or adding
void RunningNow();


//bools sent to kernel to tell it whether the order succeed or failed
void FailOrSuccessAdd();
void FailOrSuccessDelete();


//it is sent to the kernel to tell it how many slots available
int getStatus();
void RunDisk();

//signal2 handler
void SIGUSR2Handler(int signum);
void SIGUSR1Handler(int signum);

int send_message_ToKernel(int msgqid, long mtype, const char* message);
long receive_message_fromKernel(int msgqid, long mtype, char* buffer, size_t buffer_size);
void sendTestMessageToDiskDownQueue() ;
#endif
