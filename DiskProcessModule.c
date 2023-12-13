#include "DiskProcessModule.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "initqueues.h"

///////////////////////*                       variables                  *////////////////////////////


static char diskSlots[10][64];
static bool addSuccess = false;
static bool deleteSuccess = false;
volatile sig_atomic_t CLK = 0; 
static int emptySlots = 10;
static bool ISfree = true; 
static int runningtime=0;
static int Diskdown_queue;
static int Diskup_queue;
struct msgbuff recmsg;
struct msgbuff sentmsg;

///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////*                       Functions                  *////////////////////////////

void RunDisk()
{
    signal(SIGUSR2, SIGUSR2Handler);
    signal(SIGUSR1, SIGUSR1Handler);
    Diskup_queue = init_up_queue();
    Diskdown_queue = init_down_queue();
    sendTestMessageToDiskDownQueue();
    int add = 2;
    int delete = 3;
    int success= 2;
    int fail= 3;

    
        if (ISfree == true)
        {
            char receivedMessage[64];
            memset(receivedMessage, '\0', sizeof(receivedMessage));
            receive_message_fromKernel(Diskdown_queue, add, receivedMessage, sizeof(receivedMessage));
            if (receivedMessage[0] != '\0') 
            {
                addDataToDisk(receivedMessage);
                printf("Received message: %s\n", receivedMessage);
             } 
        else
        {
            receive_message_fromKernel(Diskdown_queue, delete, receivedMessage, sizeof(receivedMessage));
            
            printf("Received an empty message.\n");
         
        }
            
        
       
        }

    
}
void RunningNow(int type)
{
    if(type ==1)
    {
        if(runningtime==3)
        {
            ISfree=true;
            return;
        }
    }
    else
    {
        if(runningtime==1)
        {
            ISfree=true;
            return;
        }
    }
    
    runningtime++;
    ISfree=false;
}


void addDataToDisk(const char* data) 
{
    int emppty=0;
    if (ISfree == true) {
        for (int i = 0; i < 10; i++) {
            if (diskSlots[i][0] == '\0')
            {
                emppty=i;
                
            }
        }
        for (int j = 0; j < 64; j++) 
        {
                
            diskSlots[emppty][j] = data[j];
        }
    }
}
void deleteDataFromDisk(int slotIndex) {
    if(ISfree==true)
    {
        if (slotIndex >= 0 && slotIndex < MAX_DISK_SLOTS) {
           for (int j = 0; j < 64; j++) 
            {
                    
                diskSlots[slotIndex][j] = '\0' ;
            }
        }
    } 
    else {
        deleteSuccess = false;
    }
}

void FailOrSuccessAdd() {


    if (addSuccess==true)
    {
        
        //send_message_ToKernel(Diskup_queue, 1, "1");
    }
    else
    {
        
        //send_message_ToKernel(Diskup_queue, 1, "0");
    }
}

void FailOrSuccessDelete() 
{
     if (deleteSuccess==true)
    {
        send_message_ToKernel(Diskup_queue, 1, "1");
    }
    else
    {
        send_message_ToKernel(Diskup_queue, 1, "0");
    }
}

int getStatus()
 {
    for (int i = 0; i < MAX_DISK_SLOTS; i++) {
        
    }
    return emptySlots;
}

void SIGUSR2Handler(int signum) {
    
    CLK++;
    
}
void  SIGUSR1Handler(int signum) {
    
    int available_slots = getStatus();
    char available_slots_str[10];
    snprintf(available_slots_str, sizeof(available_slots_str), "%d", available_slots);
    send_message_ToKernel(Diskup_queue, 0, available_slots_str);

 
}


int send_message_ToKernel(int msgqid, long mtype, const char* message) {

    sentmsg.mtype = mtype;
    strncpy(sentmsg.mtext, message, sizeof(sentmsg.mtext) - 1);
    sentmsg.mtext[sizeof(sentmsg.mtext) - 1] = '\0';

    if (msgsnd(msgqid, &sentmsg, sizeof(sentmsg.mtext), 1) == -1) {
        perror("Error in sending message");
        return -1;
    }
    printf("sent message");
    return 0;
}


long receive_message_fromKernel(int msgqid, long mtype, char* buffer, size_t buffer_size) {

    int ohflag;
    if (msgrcv(msgqid, &recmsg, sizeof(recmsg.mtext), mtype, 1) == -1) {
        perror("Error in receiving message");
        return -1;
    }
    mtype=  recmsg.mtype;
    strncpy(buffer, recmsg.mtext, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
    printf("recieve message");
    return ohflag;
}

void sendTestMessageToDiskDownQueue() {
    const char* testMessage1 = "ana te3bt wallahy ana 1";
    const char* testMessage2 = "ana te3bt wallahy ana 2";
    int Diskdown_queue = init_down_queue();
    send_message_ToKernel(Diskdown_queue, 3, testMessage1);
    send_message_ToKernel(Diskdown_queue, 2, testMessage2);

}