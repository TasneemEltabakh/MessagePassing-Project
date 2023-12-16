#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main() {
    // Find the PID of the disk process using pgrep
    FILE *pgrep_output = popen("pgrep -f 'Disk.out'", "r");
    char pid_str[20];

    if (fgets(pid_str, sizeof(pid_str), pgrep_output) != NULL) {
        // Remove newline character from the end
        pid_str[strcspn(pid_str, "\n")] = '\0';

        // Convert PID string to integer
        pid_t disk_pid = atoi(pid_str);

        // Send SIGUSR1 signal to the disk process
        if (kill(disk_pid, SIGUSR2) == 0) {
            printf("Signal sent successfully to disk process (PID: %d)\n", disk_pid);
        } else {
            perror("Error sending signal");
        }
    } else {
        printf("Disk process not found\n");
    }

    // Close the pgrep process
    pclose(pgrep_output);

    return 0;
}