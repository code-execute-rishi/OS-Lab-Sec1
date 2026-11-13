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
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int n;

    printf("========================================================\n");
    printf("   OS Lab 5 (b1): Generating N Child Processes         \n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111     \n");
    printf("========================================================\n");

    // get N from argument or from the user
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

    int parent = getpid();
    printf("\n[ROOT PARENT] PID = %d is initializing spawn loop for %d children...\n", parent, n);
    printf("----------------------------------------------------------------------\n");
    printf("%-10s %-12s %-14s %-14s %-16s\n", "Child #", "Status", "Child PID", "Parent PID", "PPID (getppid)");
    printf("----------------------------------------------------------------------\n");

    // parent forks N times
    for (int i = 1; i <= n; i++) {
        fflush(stdout);  // flush before fork so the child doesn't repeat old output
        if (fork() == 0) {
            // child: print its row and exit (so it does not fork more)
            printf("%-10d %-12s %-14d %-14d %-16d\n", i, "ACTIVE", getpid(), parent, getppid());
            exit(i * 10);
        }
    }

    // parent: wait for every child (no zombies)
    printf("----------------------------------------------------------------------\n");
    printf("[PARENT] Waiting for all child processes to reap exit status...\n");

    int status, pid;
    while ((pid = wait(&status)) > 0)
        printf("[REAPED] Child PID %-6d terminated normally with exit code %d\n", pid, WEXITSTATUS(status));

    printf("========================================================\n");
    printf("All %d child processes created, executed, and reaped successfully!\n", n);
    return 0;
}
