
#include "DiskProcessModule.h"
#include <stdint.h>

volatile sig_atomic_t CLK = 0;
int Diskdown_queue;
int Diskup_queue;
int pid;
char diskSlots[10][64];
struct msgbuff
{
    long mtype;
    char mtext[64];
    int rtype;
};

// ///////////////////////*                       Functions                  *////////////////////////////

void initializeslots()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            diskSlots[i][j] = '\0';
        }
    }
}

void Disk(key_t up, key_t down)
{
    int checkvalue= -1;
    int rec;
    int sent;
    char msg[] = "";
    struct msgbuff diskmsg;
    struct msgbuff diskmsgsent;
    char str_value[20];
    
   
    while (1)
    {
        printf("\n");
        printf("\n");
        printf("\nThe clock now is: %d\n", CLK);
        rec= msgrcv(down, &diskmsg, sizeof(struct msgbuff), 0, !IPC_NOWAIT);
        if (rec== -1)
        {
            perror("Error in recieving");
        }
        else
        {
            printf("\nDisk recieved from Kernel: %s\n", diskmsg.mtext);
        }
        if(diskmsg.mtype==1 && diskmsg.mtext[0] !='\0')
        {
            if(rec !=-1)
            {
                checkvalue= addDataToDisk(diskmsg.mtext);
            }
            
        }
        else if(diskmsg.mtype==2)
        {
            if(rec !=-1)
            {

            checkvalue= deleteDataFromDisk(diskmsg.mtext);


            }
            
        }
        diskmsgsent.rtype=1; //stands for success or fail
        diskmsgsent.mtype= pid;
        char checkvalue_str[64];
        sprintf(checkvalue_str, "%d", checkvalue);
        strcpy(diskmsgsent.mtext, checkvalue_str);
         
       if(checkvalue !=-1 && rec !=-1)
       {
        printf("\nthe check value is: %s\n",checkvalue_str);
        sent= msgsnd(up, &diskmsgsent, sizeof(struct msgbuff),!IPC_NOWAIT );
        if (sent== -1)
        {
            perror("Error in sent");
        }
        else
        {
            printf("\nDisk sent to Kernel: %s\n", diskmsgsent.mtext);
        }


       }
        
        printf("\nEmpty slots: %d\n", getStatus());
        for (int i = 0; i < 10; i++)
        {
            printf("\nslot's %d data: %s\n", i+1, diskSlots[i]);
        }
        
        
    }
        

    
    
}

void main()
{
    pid= getpid();
    initializeslots();
    signal(SIGUSR2, SIGUSR2Handler);
    signal(SIGUSR1, SIGUSR1Handler);
    key_t id_Up_Disk, id_Down_Disk;
    
    id_Up_Disk = 202101220;
    id_Down_Disk = 292002;
    Diskup_queue = msgget(id_Up_Disk, IPC_CREAT | 0644);
    Diskdown_queue = msgget(id_Down_Disk, IPC_CREAT | 0644);

    Disk(Diskup_queue,Diskdown_queue);

}

 /*
“0” i.e. successful ADD.

“1” i.e. successful DEL.

“2” i.e. unable to ADD.

“3” i.e. unable to DEL.*/

int addDataToDisk(const char *data)
{
    int oldclk=CLK;
    int timer=0;
    int emppty = -1;
    int diff= CLK-oldclk;
    while(diff<3)
    {
        diff=CLK-oldclk;
    }
    for (int i = 0; i < 10; i++)
    {
        if (diskSlots[i][0] == '\0')
        {
            emppty = i;
            break;
        }
    }
    for (int j = 0; j < 64; j++)
    {

        diskSlots[emppty][j] = data[j];
    }
    timer=0;
  
    if(emppty==-1)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
int deleteDataFromDisk(char *slot)
{
    // Remove quotation marks from the slot string
    char *cleanedSlot = strtok(slot, "\"");

    int oldclk = CLK;
    int timer = 0;
    int emppty = -1;
    int diff = CLK - oldclk;

    // Convert cleanedSlot to an integer
    int slotIndex = atoi(cleanedSlot);

    while (diff < 1)
    {
        diff = CLK - oldclk;
    }

    timer = 0;

    if (slotIndex >= 0 && slotIndex < MAX_DISK_SLOTS)
    {
        if (diskSlots[slotIndex][0] != '\0')
        {
            for (int j = 0; j < 64; j++)
            {
                diskSlots[slotIndex][j] = '\0';
            }

            return 1; // Successful deletion
        }
        else
        {
            return 3; // Empty slot
        }
    }
    else
    {
        return 3; // Invalid slot index
    }
}

int getStatus()
{
    int empty = 10;
    for (int i = 0; i < MAX_DISK_SLOTS; i++)
    {
        if (diskSlots[i][0] != '\0')
        {
            empty--;
        }
    }
    return empty;
}

void SIGUSR2Handler(int signum)
{
    printf("\n  SIGUSR2 SENT \n");
    CLK++;
    signal(SIGUSR2,SIGUSR2Handler);
}
void SIGUSR1Handler(int signum)
{
    int sentval;
    struct msgbuff msg;
    printf("\n  SIGUSR1 SENT \n");
    int available_slots = getStatus();
    msg.mtype= 2;
    msg.rtype= 2; //stands for status
    sprintf(msg.mtext, "%d", available_slots);
    sentval= msgsnd(Diskup_queue, &msg, sizeof(struct msgbuff),!IPC_NOWAIT );
    if (sentval== -1)
    {
        perror("Error in sending");
    }
    else
    {
        printf("\nDisk sent to Kernel: the status %s", msg.mtext);
    }
     signal(SIGUSR1, SIGUSR1Handler);

}
