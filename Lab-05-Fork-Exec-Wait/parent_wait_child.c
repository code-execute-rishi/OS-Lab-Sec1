/**
 * ============================================================================
 * Operating Systems Laboratory (CS-502) - Experiment 5 (Part b2)
 * Student Name  : Aradhya Jain
 * Scholar No    : 24112011111
 * Section       : CSE-1
 *
 * Program: Process Synchronization Demonstrating Parent Waiting for Child
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(void) {
    printf("========================================================\n");
    printf("   OS Lab 5 (b2): Parent Waiting for Child Completion   \n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111     \n");
    printf("========================================================\n");

    pid_t parent_pid = getpid();
    printf("[1. PARENT] Starting execution. Parent PID: %d\n", parent_pid);
    printf("[1. PARENT] Invoking fork() to create a dedicated worker child...\n\n");

    fflush(stdout); /* flush before fork() so buffered text is not copied into the child */
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Error: fork() failed: %s\n", strerror(errno));
        return 1;
    }

    if (pid == 0) {
        /* Child process execution */
        printf("  [CHILD] Hello from child process!\n");
        printf("  [CHILD] My PID       : %d\n", getpid());
        printf("  [CHILD] My Parent PID: %d\n", getppid());
        printf("  [CHILD] Performing intensive task (simulated with 2s sleep)...\n");

        for (int i = 1; i <= 2; i++) {
            sleep(1);
            printf("  [CHILD] Working... elapsed %d second(s)\n", i);
        }

        printf("  [CHILD] Task completed successfully. Exiting with exit code 42.\n");
        exit(42);
    } else {
        /* Parent process execution */
        printf("[PARENT] Child created with PID: %d\n", pid);
        printf("[PARENT] Entering wait(&status) - blocking until child completes...\n\n");

        int status;
        pid_t completed_pid = wait(&status);

        printf("\n[PARENT] Resumed after wait()!\n");
        printf("[PARENT] Child PID returned by wait(): %d\n", completed_pid);

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("[PARENT] Normal termination detected (WIFEXITED == true).\n");
            printf("[PARENT] Child exited with return status code: %d\n", exit_code);
            if (exit_code == 42) {
                printf("[PARENT] Status matches expected child verification code (42).\n");
            }
        } else if (WIFSIGNALED(status)) {
            printf("[PARENT] Child was terminated abruptly by signal %d (WTERMSIG).\n",
                   WTERMSIG(status));
        }

        printf("========================================================\n");
        printf("Parent-child lifecycle synchronization concluded cleanly.\n");
    }

    return 0;
}
