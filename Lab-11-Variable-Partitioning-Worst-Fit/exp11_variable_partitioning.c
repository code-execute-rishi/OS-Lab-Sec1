/*
 * ==============================================================================
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 11
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * Semester       : V Semester, B.Tech CSE
 * Institution    : Maulana Azad National Institute of Technology (MANIT) Bhopal
 * ==============================================================================
 * AIM: Implementation of Variable Partitioning Memory Placement (Worst Fit)
 *      with Timely Ordered Dynamic Arrival and Departure Events.
 * ==============================================================================
 */

#include <stdio.h>

#define RAM 1024
#define MAX 50

/* Memory = list of segments in address order */
int start[MAX], size[MAX], owner[MAX];   /* owner = process number, 0 = FREE hole */
int n;                                   /* number of segments */

void resetMemory() {
    n = 1;
    start[0] = 0;
    size[0] = RAM;
    owner[0] = 0;
}

/* Worst Fit: put process in the LARGEST free hole, split off the leftover */
int worstFitAllocate(int pid, int need) {
    int w = -1;
    for (int i = 0; i < n; i++)
        if (owner[i] == 0 && size[i] >= need)
            if (w == -1 || size[i] > size[w])
                w = i;
    if (w == -1)
        return 0;   /* no hole big enough */

    if (size[w] > need) {
        /* shift right to make room for the leftover hole at w+1 */
        for (int k = n; k > w + 1; k--) {
            start[k] = start[k - 1];
            size[k] = size[k - 1];
            owner[k] = owner[k - 1];
        }
        start[w + 1] = start[w] + need;
        size[w + 1] = size[w] - need;
        owner[w + 1] = 0;
        n++;
        size[w] = need;
    }
    owner[w] = pid;
    return 1;
}

/* Free the process, then merge neighbouring free holes */
void release(int pid) {
    for (int i = 0; i < n; i++)
        if (owner[i] == pid)
            owner[i] = 0;

    int i = 0;
    while (i < n - 1) {
        if (owner[i] == 0 && owner[i + 1] == 0) {
            size[i] += size[i + 1];
            /* shift left to remove segment i+1 */
            for (int k = i + 1; k < n - 1; k++) {
                start[k] = start[k + 1];
                size[k] = size[k + 1];
                owner[k] = owner[k + 1];
            }
            n--;
        } else {
            i++;
        }
    }
}

int totalFree() {
    int sum = 0;
    for (int i = 0; i < n; i++)
        if (owner[i] == 0)
            sum += size[i];
    return sum;
}

void printMap(char *title) {
    int used = 0, holes = 0;
    printf("  >>> Event: %-32s <<<\n", title);
    printf("  Memory Layout Map: [");
    for (int i = 0; i < n; i++) {
        if (owner[i] == 0) {
            holes++;
            printf("| FREE:%dKB (%d-%d) |", size[i], start[i], start[i] + size[i]);
        } else {
            used += size[i];
            printf("| P%d:%dKB (%d-%d) |", owner[i], size[i], start[i], start[i] + size[i]);
        }
    }
    printf("]\n");
    printf("  Occupied: %d KB | Available: %d KB in %d free hole(s)\n\n", used, RAM - used, holes);
}

void runCase1() {
    printf("########################################################################################\n");
    printf("  EXPERIMENT 11 - CASE 1: SIMULTANEOUS ARRIVAL AT TIME T\n");
    printf("  Total RAM: 1024 KB | Processes: P1(100KB), P2(200KB), P3(300KB), P4(300KB), P5(300KB)\n");
    printf("########################################################################################\n\n");

    resetMemory();
    printMap("Initial State (Single 1024 KB Hole)");

    int sizes[5] = {100, 200, 300, 300, 300};
    for (int i = 0; i < 5; i++) {
        int pid = i + 1;
        if (worstFitAllocate(pid, sizes[i])) {
            char title[64];
            sprintf(title, "Arrival of P%d (%d KB)", pid, sizes[i]);
            printMap(title);
        } else {
            /* external fragmentation = free memory, only if it was enough in total */
            int freeMem = totalFree();
            int ext = 0;
            if (freeMem >= sizes[i])
                ext = freeMem;
            printf("  [ALERT] Failed to allocate P%d (%d KB) under Worst Fit!\n", pid, sizes[i]);
            printf("  Total Free Memory: %d KB | Required: %d KB | External Fragmentation: %d KB\n\n",
                   freeMem, sizes[i], ext);
        }
    }
}

void runCase2() {
    printf("########################################################################################\n");
    printf("  EXPERIMENT 11 - CASE 2: TIMELY ORDERED ARRIVALS AND DEPARTURES (T1 TO T8)\n");
    printf("  Total RAM: 1024 KB | Demonstrating Dynamic Fragmentation and Hole Creation\n");
    printf("########################################################################################\n\n");

    resetMemory();
    printMap("Initial State: 1024 KB Free Hole");

    /* Event at time T(i+1): arrive (1) or leave (0) */
    int arrive[8] = {1, 1, 1, 0, 1, 1, 0, 1};
    int pid[8]    = {1, 2, 3, 2, 4, 5, 4, 6};
    int sz[8]     = {100, 100, 200, 100, 200, 200, 200, 300};

    for (int i = 0; i < 8; i++) {
        char title[80];
        if (!arrive[i]) {
            sprintf(title, "T%d: P%d terminates/leaves (%d KB)", i + 1, pid[i], sz[i]);
            release(pid[i]);
            printMap(title);
            continue;
        }

        sprintf(title, "T%d: P%d arrives (%d KB)", i + 1, pid[i], sz[i]);
        if (worstFitAllocate(pid[i], sz[i])) {
            printMap(title);
        } else {
            int freeMem = totalFree();
            int ext = 0;
            if (freeMem >= sz[i])
                ext = freeMem;
            printf("  >>> Event: %-32s <<<\n", title);
            printf("  [FAILED ALLOCATION] No single contiguous hole >= %d KB!\n", sz[i]);
            printf("  Total Free Memory Available = %d KB\n", freeMem);
            printf("  Process Size Required       = %d KB\n", sz[i]);
            printf("  External Fragmentation      = %d KB\n", ext);
            printf("  -> NOTE: Free space (%d KB) exceeds required (%d KB), but is non-contiguous!\n\n",
                   freeMem, sz[i]);
        }
    }
}

int main() {
    printf("========================================================================================\n");
    printf("   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 11\n");
    printf("   VARIABLE PARTITIONING MEMORY PLACEMENT: WORST FIT SIMULATION\n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("========================================================================================\n\n");

    runCase1();
    runCase2();

    return 0;
}
