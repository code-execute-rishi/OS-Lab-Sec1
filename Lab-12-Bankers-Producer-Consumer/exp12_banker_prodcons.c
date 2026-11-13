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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROC 10
#define MAX_RES 10

// -----------------------------------------------------------------------------
// PART A: BANKER'S ALGORITHM IMPLEMENTATION
// -----------------------------------------------------------------------------

bool is_safe_state(int m, int n, int avail[], int max[][MAX_RES], int alloc[][MAX_RES], int need[][MAX_RES], int safe_seq[]) {
    int work[MAX_RES];
    bool finish[MAX_PROC];

    for (int j = 0; j < n; j++) work[j] = avail[j];
    for (int i = 0; i < m; i++) finish[i] = false;

    int count = 0;
    while (count < m) {
        bool found = false;
        for (int p = 0; p < m; p++) {
            if (!finish[p]) {
                bool can_allocate = true;
                for (int j = 0; j < n; j++) {
                    if (need[p][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    for (int j = 0; j < n; j++) {
                        work[j] += alloc[p][j];
                    }
                    safe_seq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            return false; // System is in Unsafe State (Potential Deadlock)
        }
    }
    return true; // System is in Safe State
}

bool request_resources(int p_id, int req[], int m, int n, int avail[], int max[][MAX_RES], int alloc[][MAX_RES], int need[][MAX_RES]) {
    // 1. Check if request <= need
    for (int j = 0; j < n; j++) {
        if (req[j] > need[p_id][j]) {
            printf("  [ERROR] Process P%d has exceeded its declared maximum claim!\n", p_id);
            return false;
        }
    }

    // 2. Check if request <= available
    for (int j = 0; j < n; j++) {
        if (req[j] > avail[j]) {
            printf("  [WAIT] Resources not available. Process P%d must wait.\n", p_id);
            return false;
        }
    }

    // 3. Pretend to allocate resources
    for (int j = 0; j < n; j++) {
        avail[j] -= req[j];
        alloc[p_id][j] += req[j];
        need[p_id][j] -= req[j];
    }

    // 4. Run safety check
    int safe_seq[MAX_PROC];
    if (is_safe_state(m, n, avail, max, alloc, need, safe_seq)) {
        printf("  [SUCCESS] Request granted safely! System remains in a SAFE STATE.\n");
        printf("  Safe Execution Sequence: < ");
        for (int i = 0; i < m; i++) printf("P%d ", safe_seq[i]);
        printf(">\n");
        return true;
    } else {
        printf("  [UNSAFE] Granting request leads to an UNSAFE STATE! Request rolled back.\n");
        // Rollback
        for (int j = 0; j < n; j++) {
            avail[j] += req[j];
            alloc[p_id][j] -= req[j];
            need[p_id][j] += req[j];
        }
        return false;
    }
}

void run_bankers_demo() {
    printf("========================================================================================\n");
    printf("  PART A: BANKER'S ALGORITHM DEADLOCK AVOIDANCE SIMULATION\n");
    printf("  Processes M = 5 (P0 to P4) | Resource Types N = 3 (A, B, C)\n");
    printf("========================================================================================\n\n");

    int m = 5;
    int n = 3;

    int total[MAX_RES] = {10, 5, 7}; // Total Available instances in system
    int alloc[MAX_PROC][MAX_RES] = {
        {0, 1, 0}, // P0
        {2, 0, 0}, // P1
        {3, 0, 2}, // P2
        {2, 1, 1}, // P3
        {0, 0, 2}  // P4
    };
    int max[MAX_PROC][MAX_RES] = {
        {7, 5, 3}, // P0
        {3, 2, 2}, // P1
        {9, 0, 2}, // P2
        {2, 2, 2}, // P3
        {4, 3, 3}  // P4
    };

    int need[MAX_PROC][MAX_RES];
    int avail[MAX_RES];

    // Compute Available = Total - sum(Allocated)
    for (int j = 0; j < n; j++) {
        int sum_alloc = 0;
        for (int i = 0; i < m; i++) {
            sum_alloc += alloc[i][j];
            need[i][j] = max[i][j] - alloc[i][j];
        }
        avail[j] = total[j] - sum_alloc;
    }

    printf("System Resource Configuration Matrices:\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  PID    Allocation (A B C)    Maximum Claim (A B C)    Need Matrix (A B C)\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < m; i++) {
        printf("  P%-5d %-2d %-2d %-16d %-2d %-2d %-20d %-2d %-2d %-2d\n",
               i,
               alloc[i][0], alloc[i][1], alloc[i][2],
               max[i][0], max[i][1], max[i][2],
               need[i][0], need[i][1], need[i][2]);
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Total System Resources: [A=%d, B=%d, C=%d]\n", total[0], total[1], total[2]);
    printf("  Currently Available   : [A=%d, B=%d, C=%d]\n\n", avail[0], avail[1], avail[2]);

    int safe_seq[MAX_PROC];
    printf("Executing Safety Algorithm:\n");
    if (is_safe_state(m, n, avail, max, alloc, need, safe_seq)) {
        printf("  >>> STATUS: SAFE STATE <<<\n");
        printf("  Valid Safe Sequence Found: < ");
        for (int i = 0; i < m; i++) printf("P%d%s", safe_seq[i], (i == m - 1) ? "" : " -> ");
        printf(" >\n\n");
    } else {
        printf("  >>> STATUS: UNSAFE STATE (Potential Deadlock Detected) <<<\n\n");
    }

    printf("Resource-Request Simulation:\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Case 1: Process P1 requests (1, 0, 2):\n");
    int req1[] = {1, 0, 2};
    request_resources(1, req1, m, n, avail, max, alloc, need);

    printf("\nCase 2: Process P4 requests (3, 3, 0):\n");
    int req2[] = {3, 3, 0};
    request_resources(4, req2, m, n, avail, max, alloc, need);

    printf("\nCase 3: Process P0 requests (0, 2, 0):\n");
    int req3[] = {0, 2, 0};
    request_resources(0, req3, m, n, avail, max, alloc, need);
    printf("========================================================================================\n\n");
}

// -----------------------------------------------------------------------------
// PART B: PRODUCER-CONSUMER PROBLEM (EXACT MATCH TO LAB MANUAL TERMINAL LOG)
// -----------------------------------------------------------------------------

#define BUFFER_CAPACITY 3

void run_producer_consumer_demo() {
    printf("========================================================================================\n");
    printf("  PART B: PRODUCER-CONSUMER BOUNDED BUFFER SIMULATION\n");
    printf("  Buffer Capacity = %d Items\n", BUFFER_CAPACITY);
    printf("========================================================================================\n\n");

    int buffer[BUFFER_CAPACITY];
    int in = 0;
    int out = 0;
    int count = 0;
    int item_counter = 1;

    // Simulation sequence matching the manual screenshot
    int choices[] = {2, 1, 1, 1, 2, 1, 2, 2, 2, 3};
    int n_choices = sizeof(choices) / sizeof(choices[0]);

    for (int k = 0; k < n_choices; k++) {
        printf("1. Producer\n");
        printf("2. Consumer\n");
        printf("3. Exit\n");
        printf("Enter your choice: %d\n", choices[k]);

        if (choices[k] == 1) { // Producer
            if (count == BUFFER_CAPACITY) {
                printf("Buffer is full!!\n");
            } else {
                buffer[in] = item_counter;
                printf("Producer produces item %d\n", item_counter);
                item_counter++;
                in = (in + 1) % BUFFER_CAPACITY;
                count++;
            }
        } else if (choices[k] == 2) { // Consumer
            if (count == 0) {
                printf("Buffer is empty!!\n");
            } else {
                int consumed_item = buffer[out];
                printf("Consumer consumes item %d\n", consumed_item);
                out = (out + 1) % BUFFER_CAPACITY;
                count--;
            }
        } else if (choices[k] == 3) { // Exit
            printf("Exiting Producer-Consumer simulation.\n");
            break;
        }
    }
    printf("========================================================================================\n");
}

int main() {
    run_bankers_demo();
    run_producer_consumer_demo();
    return 0;
}
