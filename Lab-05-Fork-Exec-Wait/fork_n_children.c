/**
 * ============================================================================
 * Operating Systems Laboratory (CS-502) - Experiment 5 (Part b1)
 * Student Name  : Aradhya Jain
 * Scholar No    : 24112011111
 * Section       : CSE-1
 *
 * Program: Generation of N Child Processes using fork() System Call
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int n = 0;

    printf("========================================================\n");
    printf("   OS Lab 5 (b1): Generating N Child Processes         \n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111     \n");
    printf("========================================================\n");

    if (argc >= 2) {
        n = atoi(argv[1]);
    } else {
        printf("Enter the number of child processes to generate (N): ");
        if (scanf("%d", &n) != 1) {
            fprintf(stderr, "Error: Invalid number entered.\n");
            return 1;
        }
    }

    if (n <= 0) {
        fprintf(stderr, "Error: Please enter a positive integer N > 0.\n");
        return 1;
    }

    pid_t parent_pid = getpid();
    printf("\n[ROOT PARENT] PID = %d is initializing spawn loop for %d children...\n", parent_pid, n);
    printf("----------------------------------------------------------------------\n");
    printf("%-10s %-12s %-14s %-14s %-16s\n", "Child #", "Status", "Child PID", "Parent PID", "PPID (getppid)");
    printf("----------------------------------------------------------------------\n");

    for (int i = 1; i <= n; i++) {
        fflush(stdout); /* flush before fork() so buffered text is not copied into the child */
        pid_t pid = fork();

        if (pid < 0) {
            /* Fork failure */
            fprintf(stderr, "Fork failed for iteration %d: %s\n", i, strerror(errno));
            break;
        } else if (pid == 0) {
            /* Inside Child Process */
            printf("%-10d %-12s %-14d %-14d %-16d\n",
                   i, "ACTIVE", getpid(), parent_pid, getppid());
            fflush(stdout);
            /* Child terminates immediately so it doesn't loop and spawn grandchildren */
            exit(i * 10);
        } else {
            /* Inside Parent Process: loop continues to spawn the next sibling */
        }
    }

    /* Parent waits for all N children to prevent zombie processes */
    int status;
    pid_t terminated_child;
    printf("----------------------------------------------------------------------\n");
    printf("[PARENT] Waiting for all child processes to reap exit status...\n");

    while ((terminated_child = wait(&status)) > 0) {
        if (WIFEXITED(status)) {
            printf("[REAPED] Child PID %-6d terminated normally with exit code %d\n",
                   terminated_child, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[REAPED] Child PID %-6d killed by signal %d\n",
                   terminated_child, WTERMSIG(status));
        }
    }

    printf("========================================================\n");
    printf("All %d child processes created, executed, and reaped successfully!\n", n);
    return 0;
}
