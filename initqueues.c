#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>



int init_up_queue() {
    
    int up_key_id = 292002;
    int msgqid = msgget(up_key_id, IPC_CREAT | 0644);
    if (msgqid == -1) {
        perror("Error in create");
        exit(-1);
    }
    printf("UP msgqid = %d\n", msgqid);
    return msgqid;
}


int init_down_queue() {
   
    int down_key_id = 2412019;
    int msgqid = msgget(down_key_id, IPC_CREAT | 0644);
    if (msgqid == -1) {
        perror("Error in create");
        exit(-1);
    }
    printf("DOWN msgqid = %d\n", msgqid);
    return msgqid;
}

