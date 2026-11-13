/*
 * ==============================================================================
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 12
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * Semester       : V Semester, B.Tech CSE
 * Institution    : Maulana Azad National Institute of Technology (MANIT) Bhopal
 * ==============================================================================
 * AIM: Simulation of Banker's Algorithm for Deadlock Avoidance &
 *      Simulation of the Classic Producer-Consumer Bounded Buffer Problem.
 * ==============================================================================
 */

#include <stdio.h>

#define M 5   // processes P0..P4
#define N 3   // resource types A, B, C

// ---------------- PART A: BANKER'S ALGORITHM ----------------

int total[N] = {10, 5, 7};
int alloc[M][N] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
int max[M][N]   = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
int need[M][N];
int avail[N];
int safeSeq[M];

// Safety algorithm: keep finding a process whose need <= work
int isSafe() {
    int work[N], finish[M] = {0}, count = 0;
    for (int j = 0; j < N; j++) work[j] = avail[j];

    while (count < M) {
        int found = 0;
        for (int i = 0; i < M; i++) {
            if (finish[i]) continue;
            int j;
            for (j = 0; j < N; j++)
                if (need[i][j] > work[j]) break;
            if (j == N) {                       // process i can finish
                for (j = 0; j < N; j++) work[j] += alloc[i][j];
                finish[i] = 1;
                safeSeq[count++] = i;
                found = 1;
            }
        }
        if (!found) return 0;                   // nobody can run -> unsafe
    }
    return 1;
}

// Resource-request algorithm
void request(int p, int req[]) {
    for (int j = 0; j < N; j++)
        if (req[j] > need[p][j]) {
            printf("  [ERROR] Process P%d has exceeded its declared maximum claim!\n", p);
            return;
        }
    for (int j = 0; j < N; j++)
        if (req[j] > avail[j]) {
            printf("  [WAIT] Resources not available. Process P%d must wait.\n", p);
            return;
        }

    // pretend to allocate
    for (int j = 0; j < N; j++) {
        avail[j] -= req[j];
        alloc[p][j] += req[j];
        need[p][j] -= req[j];
    }

    if (isSafe()) {
        printf("  [SUCCESS] Request granted safely! System remains in a SAFE STATE.\n");
        printf("  Safe Execution Sequence: < ");
        for (int i = 0; i < M; i++) printf("P%d ", safeSeq[i]);
        printf(">\n");
    } else {
        printf("  [UNSAFE] Granting request leads to an UNSAFE STATE! Request rolled back.\n");
        for (int j = 0; j < N; j++) {           // roll back
            avail[j] += req[j];
            alloc[p][j] -= req[j];
            need[p][j] += req[j];
        }
    }
}

void bankers() {
    printf("========================================================================================\n");
    printf("  PART A: BANKER'S ALGORITHM DEADLOCK AVOIDANCE SIMULATION\n");
    printf("  Processes M = 5 (P0 to P4) | Resource Types N = 3 (A, B, C)\n");
    printf("========================================================================================\n\n");

    // need = max - alloc,  avail = total - sum of alloc
    for (int j = 0; j < N; j++) {
        avail[j] = total[j];
        for (int i = 0; i < M; i++) {
            need[i][j] = max[i][j] - alloc[i][j];
            avail[j] -= alloc[i][j];
        }
    }

    printf("System Resource Configuration Matrices:\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  PID    Allocation (A B C)    Maximum Claim (A B C)    Need Matrix (A B C)\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < M; i++)
        printf("  P%-5d %-2d %-2d %-16d %-2d %-2d %-20d %-2d %-2d %-2d\n", i,
               alloc[i][0], alloc[i][1], alloc[i][2],
               max[i][0], max[i][1], max[i][2],
               need[i][0], need[i][1], need[i][2]);
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Total System Resources: [A=%d, B=%d, C=%d]\n", total[0], total[1], total[2]);
    printf("  Currently Available   : [A=%d, B=%d, C=%d]\n\n", avail[0], avail[1], avail[2]);

    printf("Executing Safety Algorithm:\n");
    if (isSafe()) {
        printf("  >>> STATUS: SAFE STATE <<<\n");
        printf("  Valid Safe Sequence Found: < ");
        for (int i = 0; i < M; i++) {
            printf("P%d", safeSeq[i]);
            if (i < M - 1) printf(" -> ");
        }
        printf(" >\n\n");
    } else {
        printf("  >>> STATUS: UNSAFE STATE (Potential Deadlock Detected) <<<\n\n");
    }

    printf("Resource-Request Simulation:\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Case 1: Process P1 requests (1, 0, 2):\n");
    int req1[N] = {1, 0, 2};
    request(1, req1);

    printf("\nCase 2: Process P4 requests (3, 3, 0):\n");
    int req2[N] = {3, 3, 0};
    request(4, req2);

    printf("\nCase 3: Process P0 requests (0, 2, 0):\n");
    int req3[N] = {0, 2, 0};
    request(0, req3);
    printf("========================================================================================\n\n");
}

// ---------------- PART B: PRODUCER-CONSUMER ----------------

#define SIZE 3

void producerConsumer() {
    printf("========================================================================================\n");
    printf("  PART B: PRODUCER-CONSUMER BOUNDED BUFFER SIMULATION\n");
    printf("  Buffer Capacity = %d Items\n", SIZE);
    printf("========================================================================================\n\n");

    int buffer[SIZE], in = 0, out = 0, count = 0, item = 1;
    int choices[10] = {2, 1, 1, 1, 2, 1, 2, 2, 2, 3};

    for (int k = 0; k < 10; k++) {
        printf("1. Producer\n");
        printf("2. Consumer\n");
        printf("3. Exit\n");
        printf("Enter your choice: %d\n", choices[k]);

        if (choices[k] == 1) {                  // produce at 'in'
            if (count == SIZE) {
                printf("Buffer is full!!\n");
            } else {
                buffer[in] = item;
                printf("Producer produces item %d\n", item);
                item++;
                in = (in + 1) % SIZE;
                count++;
            }
        } else if (choices[k] == 2) {           // consume from 'out'
            if (count == 0) {
                printf("Buffer is empty!!\n");
            } else {
                printf("Consumer consumes item %d\n", buffer[out]);
                out = (out + 1) % SIZE;
                count--;
            }
        } else {
            printf("Exiting Producer-Consumer simulation.\n");
            break;
        }
    }
    printf("========================================================================================\n");
}

int main() {
    bankers();
    producerConsumer();
    return 0;
}
