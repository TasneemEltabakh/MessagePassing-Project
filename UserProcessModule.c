#include "UserProcessModule.h"

int CLK = 0;
int UPup_queue;
int UPdown_queue;
int pid;

struct msgbuff
{
    long mtype;
    char mtext[64];
    int rtype;
};

// ///////////////////////*                       Functions                  *////////////////////////////
void waitfortime(int stime)
{
    while (1)
    {
        printf("\nclk is%d\n", CLK);

        if (CLK >= stime)
        {
            break;
        }
    }
}
void clkupdate()
{
    CLK++;
}

void SIGUSR2Handler(int signum)
{
    printf("\nSIGUSR2 SENT\n");
    clkupdate();
    signal(SIGUSR2, SIGUSR2Handler);
}

void readRequestsFromFile(const char *filename)
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char line[70];
    while (fgets(line, sizeof(line), file))
    {

        printf("\nThe clock now is: %d\n", CLK);
        int time = 0;
        char operation[10];
        char data[64];
        sscanf(line, "%d %s  %[^\n]", &time, operation, data);
        while (CLK < time)
        {
           
           
        }

        char request[80];
        struct msgbuff msg;
        if (strcmp(operation, "ADD") == 0)
        {
            strcpy(msg.mtext, data);
            msg.mtype = 1;
            printf("operation: %s\n", operation);
            printf("text: %s\n", data);
           
        }
        else if (strcmp(operation, "DEL") == 0)
        {
            strcpy(msg.mtext, data);
            msg.mtype = 2;
            printf("operation: %s\n", operation);
            printf("text: %s\n", data);
        }
        else
        {
            printf("Invalid operation: %s\n", operation);
            continue;
        }
        msg.rtype= getpid();
         printf("rtype: %d\n", msg.rtype);
        int sentval = msgsnd(UPup_queue, &msg, sizeof(struct msgbuff), !IPC_NOWAIT);

        if (sentval == -1)
        {
            perror("Error in sending");
        }
        else
        {
            printf("\nUser Process sent to Kernel: %s", msg.mtext);
        }

        struct msgbuff response;
        int rec = msgrcv(UPdown_queue, &response, sizeof(struct msgbuff), pid, !IPC_NOWAIT);
        if (rec == -1)
        {
            perror("Error in receiving");
        }
        else
        {
            printf("\nUser Process received from Kernel: %s", response.mtext);
        }
    }

    fclose(file);
}

int main()
{
    pid = getpid();
    signal(SIGUSR2, SIGUSR2Handler);

    key_t id_Up_UP, id_Down_UP;

    id_Up_UP = 202101031;
    id_Down_UP = 202101510;
    UPup_queue = msgget(id_Up_UP, IPC_CREAT | 0644);
    UPdown_queue = msgget(id_Down_UP, IPC_CREAT | 0644);

    char filename[256];
    printf("Enter the filename: ");
    scanf("%255s", filename);

    readRequestsFromFile(filename);

    return 0;
}
