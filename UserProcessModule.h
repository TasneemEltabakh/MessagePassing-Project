#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
void sendRequestToKernel(const char *request);
void SIGUSR2Handler(int signum);
void readRequestsFromFile(const char *filename);
int main();
void clkupdate();