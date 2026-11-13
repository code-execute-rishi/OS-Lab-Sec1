/*
 * ==============================================================================
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 10
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * Semester       : V Semester, B.Tech CSE
 * Institution    : Maulana Azad National Institute of Technology (MANIT) Bhopal
 * ==============================================================================
 * AIM: Implementation of Fixed Memory Partitioning Placement Schemes
 *      (First Fit & Best Fit) with Equal and Unequal Block Sizes.
 * ==============================================================================
 */

#include <stdio.h>

#define N 4   /* 4 blocks and 4 processes */

int blockSize[N];
int procSize[N] = {100, 150, 200, 250};
int owner[N];     /* owner[j] = process index in block j, -1 if free */

/* First Fit: first free block that is big enough */
int firstFit(int size) {
    for (int j = 0; j < N; j++)
        if (owner[j] == -1 && blockSize[j] >= size)
            return j;
    return -1;
}

/* Best Fit: smallest free block that is big enough */
int bestFit(int size) {
    int best = -1;
    for (int j = 0; j < N; j++)
        if (owner[j] == -1 && blockSize[j] >= size)
            if (best == -1 || blockSize[j] < blockSize[best])
                best = j;
    return best;
}

void simulate(int blocks[], int useBest, char *title) {
    int placed[N];   /* placed[i] = 1 if process i got a block */
    int internal = 0, freeMem = 0, external = 0;

    for (int j = 0; j < N; j++) {
        blockSize[j] = blocks[j];
        owner[j] = -1;
    }

    /* Allocate every process in order */
    for (int i = 0; i < N; i++) {
        int j;
        if (useBest)
            j = bestFit(procSize[i]);
        else
            j = firstFit(procSize[i]);
        placed[i] = (j != -1);
        if (j != -1)
            owner[j] = i;
    }

    if (useBest)
        printf(">>> BEST FIT ALGORITHM - %s <<<\n", title);
    else
        printf(">>> FIRST FIT ALGORITHM - %s <<<\n", title);
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Block ID   Capacity(KB)   Occupant   Process Size(KB)   Internal Frag(KB)   Status\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int j = 0; j < N; j++) {
        if (owner[j] != -1) {
            int p = owner[j];
            int waste = blockSize[j] - procSize[p];   /* internal fragmentation */
            internal += waste;
            printf("  Block %-4d %-14d P%-8d %-18d %-19d ALLOCATED\n",
                   j + 1, blockSize[j], p + 1, procSize[p], waste);
        } else {
            freeMem += blockSize[j];
            printf("  Block %-4d %-14d ---        ---                0 (Free Block)      FREE\n",
                   j + 1, blockSize[j]);
        }
    }

    /* External fragmentation: enough total free memory for a waiting process,
       but no single block can hold it */
    for (int i = 0; i < N; i++)
        if (!placed[i] && freeMem >= procSize[i])
            external = freeMem;

    printf("----------------------------------------------------------------------------------------\n");
    printf("  Total Internal Fragmentation: %d KB\n", internal);
    printf("  External Fragmentation       : %d KB\n", external);
    for (int i = 0; i < N; i++)
        if (!placed[i])
            printf("  [ALERT] Process P%d (%d KB) could NOT be accommodated!\n", i + 1, procSize[i]);
    printf("\n");
}

int main() {
    printf("========================================================================================\n");
    printf("   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 10\n");
    printf("   FIXED PARTITIONING MEMORY PLACEMENT: FIRST FIT & BEST FIT\n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("========================================================================================\n\n");

    /* Case 1: Equal Size Blocks */
    int case1[N] = {256, 256, 256, 256};
    printf("########################################################################################\n");
    printf("  EXPERIMENT 10 - CASE 1: EQUAL BLOCK SIZES (Total RAM = 1024 KB, 4x256 KB)\n");
    printf("  Processes: P1 = 100 KB, P2 = 150 KB, P3 = 200 KB, P4 = 250 KB\n");
    printf("########################################################################################\n\n");
    simulate(case1, 0, "CASE 1 (EQUAL SIZES)");
    simulate(case1, 1, "CASE 1 (EQUAL SIZES)");

    /* Case 2: Unequal Size Blocks */
    int case2[N] = {128, 128, 256, 512};
    printf("########################################################################################\n");
    printf("  EXPERIMENT 10 - CASE 2: UNEQUAL BLOCK SIZES (Total RAM = 1024 KB: 128, 128, 256, 512)\n");
    printf("  Processes: P1 = 100 KB, P2 = 150 KB, P3 = 200 KB, P4 = 250 KB\n");
    printf("########################################################################################\n\n");
    simulate(case2, 0, "CASE 2 (UNEQUAL SIZES)");
    simulate(case2, 1, "CASE 2 (UNEQUAL SIZES)");

    return 0;
}
