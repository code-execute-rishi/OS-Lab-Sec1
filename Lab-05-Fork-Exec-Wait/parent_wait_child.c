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
#include <sys/wait.h>

int main(void) {
    printf("========================================================\n");
    printf("   OS Lab 5 (b2): Parent Waiting for Child Completion   \n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111     \n");
    printf("========================================================\n");

    printf("[1. PARENT] Starting execution. Parent PID: %d\n", getpid());
    printf("[1. PARENT] Invoking fork() to create a dedicated worker child...\n\n");

    fflush(stdout);  // flush before fork so the child doesn't repeat old output
    int pid = fork();

    if (pid == 0) {
        // child: do 2 seconds of "work", then exit with code 42
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
    }

    // parent: block in wait() until the child finishes
    printf("[PARENT] Child created with PID: %d\n", pid);
    printf("[PARENT] Entering wait(&status) - blocking until child completes...\n\n");

    int status;
    int done = wait(&status);

    printf("\n[PARENT] Resumed after wait()!\n");
    printf("[PARENT] Child PID returned by wait(): %d\n", done);

    // read the child's exit code from status
    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        printf("[PARENT] Normal termination detected (WIFEXITED == true).\n");
        printf("[PARENT] Child exited with return status code: %d\n", code);
        if (code == 42)
            printf("[PARENT] Status matches expected child verification code (42).\n");
    }

    printf("========================================================\n");
    printf("Parent-child lifecycle synchronization concluded cleanly.\n");
    return 0;
}
