#include "KernelProcessModule.h"

volatile sig_atomic_t CLK = 0;
int msquidup_Disk, msquiddown_Disk, msquidup_User, msquidDown_User;

struct msgbuff
{
    long mtype;
    char mtext[64];
    int rtype;
};

void Kernel(key_t diskup, key_t diskdown, key_t userup, key_t userdown)
{
    int disend;
    int usersend;
    int diskrec;
    int userec;
    char msg[] = "";
    struct msgbuff diskmsg;
    struct msgbuff usermsg;
    struct msgbuff slot;

    while (1)
    {
        sendignal2ToUser();
        sendignal2ToDisk();
        
        userec = msgrcv(userup, &usermsg, sizeof(usermsg.mtext), 0, !IPC_NOWAIT);
        if (userec == -1)
        {
            perror("Error in recieving");
        }
        else
        {
            printf("\nKernel recieved from User: %s\n", usermsg.mtext);
        }
        if (usermsg.rtype == 1)
        {
            sendignal1ToDisk();
            userec = msgrcv(diskup, &slot, sizeof(slot.mtext), 0, !IPC_NOWAIT);
            int int_value = atoi(slot.mtext);
            if (int_value != 0)
            {
                disend = msgsnd(diskdown, &usermsg, sizeof(usermsg.mtext), !IPC_NOWAIT);
                if (disend == -1)
                {
                    perror("Error in sending");
                }
                else
                {
                    printf("\nKernel sent to disk: %s\n", usermsg.mtext);
                }
            }
            else
            {
                  printf("\nThere is no slots here\n");
            }
        }
        else
        {
            disend = msgsnd(diskdown, &usermsg, sizeof(usermsg.mtext), !IPC_NOWAIT);
            if (disend == -1)
            {
                perror("Error in sending");
            }
            else
            {
                printf("\nKernel sent to disk: %s\n", usermsg.mtext);
            }
        }

        diskrec = msgrcv(diskup, &diskmsg, sizeof(diskmsg.mtext), 0, !IPC_NOWAIT);
        if (diskrec == -1)
        {
            perror("Error in recieving");
        }
        else
        {
            printf("\nKernel recieved from Disk: %s\n", diskmsg.mtext);
        }

        usersend = msgsnd(userdown, &diskmsg, sizeof(diskmsg.mtext), !IPC_NOWAIT);
        if (usersend == -1)
        {
            perror("Error in sending");
        }
        else
        {
            printf("\nKernel sent to User: %s", diskmsg.mtext);
        }
    }
}
void handlersigint(int signum)
{
    msgctl(msquidup_User, IPC_RMID, NULL);
    msgctl(msquiddown_Disk, IPC_RMID, NULL);
    msgctl(msquidup_Disk, IPC_RMID, NULL);
    msgctl(msquidDown_User, IPC_RMID, NULL);
    exit(0);
}
void clkhandler()
{
    CLK++;
    signal(SIGUSR2, clkhandler);
}
int main()
{
    signal(SIGINT, handlersigint);
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, clkhandler);

    key_t id_Up_Disk, id_Down_Disk, id_Up_User, id_Down_User;
    id_Up_User = 202101031;
    id_Down_User = 202101510;
    msquidup_User = msgget(id_Up_User, IPC_CREAT | 0644);
    msquidDown_User = msgget(id_Down_User, IPC_CREAT | 0644);
    id_Up_Disk = 202101220;
    id_Down_Disk = 292002;
    msquidup_Disk = msgget(id_Up_Disk, IPC_CREAT | 0644);
    msquiddown_Disk = msgget(id_Down_Disk, IPC_CREAT | 0644);

    Kernel(msquidup_Disk, msquiddown_Disk, msquidup_User, msquidDown_User);
}
void sendignal2ToDisk()
{
    FILE *pgrep_output = popen("pgrep -f 'Disk.out'", "r");
    char pid_str[20];

    if (fgets(pid_str, sizeof(pid_str), pgrep_output) != NULL)
    {
        // Remove newline character from the end
        pid_str[strcspn(pid_str, "\n")] = '\0';

        // Convert PID string to integer
        pid_t disk_pid = atoi(pid_str);

        // Send SIGUSR1 signal to the disk process
        if (kill(disk_pid, SIGUSR2) == 0)
        {
            printf("Signal sent successfully to disk process (PID: %d)\n", disk_pid);
        }
        else
        {
            perror("Error sending signal");
        }
    }
    else
    {
        printf("Disk process not found\n");
    }

    // Close the pgrep process
    pclose(pgrep_output);
    sleep(1);
}
void sendignal2ToUser()
{
    FILE *pgrep_output = popen("pgrep -f 'U.out'", "r");
    char pid_str[20];

    if (fgets(pid_str, sizeof(pid_str), pgrep_output) != NULL)
    {
        // Remove newline character from the end
        pid_str[strcspn(pid_str, "\n")] = '\0';

        // Convert PID string to integer
        pid_t disk_pid = atoi(pid_str);

        // Send SIGUSR1 signal to the disk process
        if (kill(disk_pid, SIGUSR2) == 0)
        {
            printf("Signal sent successfully to disk process (PID: %d)\n", disk_pid);
        }
        else
        {
            perror("Error sending signal");
        }
    }
    else
    {
        printf("Disk process not found\n");
    }

    // Close the pgrep process
    pclose(pgrep_output);
    sleep(1);
}
void sendignal1ToDisk()
{
    FILE *pgrep_output = popen("pgrep -f 'Disk.out'", "r");
    char pid_str[20];

    if (fgets(pid_str, sizeof(pid_str), pgrep_output) != NULL)
    {
        // Remove newline character from the end
        pid_str[strcspn(pid_str, "\n")] = '\0';

        // Convert PID string to integer
        pid_t disk_pid = atoi(pid_str);

        // Send SIGUSR1 signal to the disk process
        if (kill(disk_pid, SIGUSR1) == 0)
        {
            printf("Signal sent successfully to disk process (PID: %d)\n", disk_pid);
        }
        else
        {
            perror("Error sending signal");
        }
    }
    else
    {
        printf("Disk process not found\n");
    }

    // Close the pgrep process
    pclose(pgrep_output);
}