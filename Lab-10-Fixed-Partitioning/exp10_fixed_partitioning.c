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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BLOCKS 20
#define MAX_PROCS 20

typedef struct {
    int block_id;
    int size;
    int allocated_pid; // -1 if free
    int process_size;
    int internal_frag;
} MemoryBlock;

typedef struct {
    int pid;
    char name[10];
    int size;
    bool allocated;
    int assigned_block;
} Process;

void simulate_first_fit(const int init_blocks[], int n_blocks, const int proc_sizes[], int n_procs, const char *case_title) {
    MemoryBlock blocks[MAX_BLOCKS];
    Process procs[MAX_PROCS];

    for (int i = 0; i < n_blocks; i++) {
        blocks[i].block_id = i + 1;
        blocks[i].size = init_blocks[i];
        blocks[i].allocated_pid = -1;
        blocks[i].process_size = 0;
        blocks[i].internal_frag = 0;
    }

    for (int i = 0; i < n_procs; i++) {
        procs[i].pid = i + 1;
        sprintf(procs[i].name, "P%d", i + 1);
        procs[i].size = proc_sizes[i];
        procs[i].allocated = false;
        procs[i].assigned_block = -1;
    }

    // First Fit Allocation
    for (int i = 0; i < n_procs; i++) {
        for (int j = 0; j < n_blocks; j++) {
            if (blocks[j].allocated_pid == -1 && blocks[j].size >= procs[i].size) {
                blocks[j].allocated_pid = procs[i].pid;
                blocks[j].process_size = procs[i].size;
                blocks[j].internal_frag = blocks[j].size - procs[i].size;
                procs[i].allocated = true;
                procs[i].assigned_block = blocks[j].block_id;
                break;
            }
        }
    }

    // Metrics computation
    int total_internal_frag = 0;
    int unallocated_memory_free = 0;
    for (int j = 0; j < n_blocks; j++) {
        if (blocks[j].allocated_pid != -1) {
            total_internal_frag += blocks[j].internal_frag;
        } else {
            unallocated_memory_free += blocks[j].size;
        }
    }

    int external_frag = 0;
    for (int i = 0; i < n_procs; i++) {
        if (!procs[i].allocated && unallocated_memory_free >= procs[i].size) {
            external_frag = unallocated_memory_free;
            break;
        }
    }

    printf(">>> FIRST FIT ALGORITHM - %s <<<\n", case_title);
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Block ID   Capacity(KB)   Occupant   Process Size(KB)   Internal Frag(KB)   Status\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int j = 0; j < n_blocks; j++) {
        if (blocks[j].allocated_pid != -1) {
            printf("  Block %-4d %-14d P%-8d %-18d %-19d ALLOCATED\n",
                   blocks[j].block_id, blocks[j].size, blocks[j].allocated_pid,
                   blocks[j].process_size, blocks[j].internal_frag);
        } else {
            printf("  Block %-4d %-14d ---        ---                0 (Free Block)      FREE\n",
                   blocks[j].block_id, blocks[j].size);
        }
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Total Internal Fragmentation: %d KB\n", total_internal_frag);
    printf("  External Fragmentation       : %d KB\n", external_frag);
    for (int i = 0; i < n_procs; i++) {
        if (!procs[i].allocated) {
            printf("  [ALERT] Process P%d (%d KB) could NOT be accommodated!\n", procs[i].pid, procs[i].size);
        }
    }
    printf("\n");
}

void simulate_best_fit(const int init_blocks[], int n_blocks, const int proc_sizes[], int n_procs, const char *case_title) {
    MemoryBlock blocks[MAX_BLOCKS];
    Process procs[MAX_PROCS];

    for (int i = 0; i < n_blocks; i++) {
        blocks[i].block_id = i + 1;
        blocks[i].size = init_blocks[i];
        blocks[i].allocated_pid = -1;
        blocks[i].process_size = 0;
        blocks[i].internal_frag = 0;
    }

    for (int i = 0; i < n_procs; i++) {
        procs[i].pid = i + 1;
        sprintf(procs[i].name, "P%d", i + 1);
        procs[i].size = proc_sizes[i];
        procs[i].allocated = false;
        procs[i].assigned_block = -1;
    }

    // Best Fit Allocation: find free block with smallest size >= process size
    for (int i = 0; i < n_procs; i++) {
        int best_idx = -1;
        int min_diff = 999999;

        for (int j = 0; j < n_blocks; j++) {
            if (blocks[j].allocated_pid == -1 && blocks[j].size >= procs[i].size) {
                int diff = blocks[j].size - procs[i].size;
                if (diff < min_diff) {
                    min_diff = diff;
                    best_idx = j;
                }
            }
        }

        if (best_idx != -1) {
            blocks[best_idx].allocated_pid = procs[i].pid;
            blocks[best_idx].process_size = procs[i].size;
            blocks[best_idx].internal_frag = blocks[best_idx].size - procs[i].size;
            procs[i].allocated = true;
            procs[i].assigned_block = blocks[best_idx].block_id;
        }
    }

    // Metrics computation
    int total_internal_frag = 0;
    int unallocated_memory_free = 0;
    for (int j = 0; j < n_blocks; j++) {
        if (blocks[j].allocated_pid != -1) {
            total_internal_frag += blocks[j].internal_frag;
        } else {
            unallocated_memory_free += blocks[j].size;
        }
    }

    int external_frag = 0;
    for (int i = 0; i < n_procs; i++) {
        if (!procs[i].allocated && unallocated_memory_free >= procs[i].size) {
            external_frag = unallocated_memory_free;
            break;
        }
    }

    printf(">>> BEST FIT ALGORITHM - %s <<<\n", case_title);
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Block ID   Capacity(KB)   Occupant   Process Size(KB)   Internal Frag(KB)   Status\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int j = 0; j < n_blocks; j++) {
        if (blocks[j].allocated_pid != -1) {
            printf("  Block %-4d %-14d P%-8d %-18d %-19d ALLOCATED\n",
                   blocks[j].block_id, blocks[j].size, blocks[j].allocated_pid,
                   blocks[j].process_size, blocks[j].internal_frag);
        } else {
            printf("  Block %-4d %-14d ---        ---                0 (Free Block)      FREE\n",
                   blocks[j].block_id, blocks[j].size);
        }
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Total Internal Fragmentation: %d KB\n", total_internal_frag);
    printf("  External Fragmentation       : %d KB\n", external_frag);
    for (int i = 0; i < n_procs; i++) {
        if (!procs[i].allocated) {
            printf("  [ALERT] Process P%d (%d KB) could NOT be accommodated!\n", procs[i].pid, procs[i].size);
        }
    }
    printf("\n");
}

int main() {
    printf("========================================================================================\n");
    printf("   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 10\n");
    printf("   FIXED PARTITIONING MEMORY PLACEMENT: FIRST FIT & BEST FIT\n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("========================================================================================\n\n");

    // Case 1: Equal Size Blocks
    int blocks_case1[] = {256, 256, 256, 256};
    int procs_case1[]  = {100, 150, 200, 250};
    printf("########################################################################################\n");
    printf("  EXPERIMENT 10 - CASE 1: EQUAL BLOCK SIZES (Total RAM = 1024 KB, 4x256 KB)\n");
    printf("  Processes: P1 = 100 KB, P2 = 150 KB, P3 = 200 KB, P4 = 250 KB\n");
    printf("########################################################################################\n\n");
    simulate_first_fit(blocks_case1, 4, procs_case1, 4, "CASE 1 (EQUAL SIZES)");
    simulate_best_fit(blocks_case1, 4, procs_case1, 4, "CASE 1 (EQUAL SIZES)");

    // Case 2: Unequal Size Blocks
    int blocks_case2[] = {128, 128, 256, 512};
    int procs_case2[]  = {100, 150, 200, 250};
    printf("########################################################################################\n");
    printf("  EXPERIMENT 10 - CASE 2: UNEQUAL BLOCK SIZES (Total RAM = 1024 KB: 128, 128, 256, 512)\n");
    printf("  Processes: P1 = 100 KB, P2 = 150 KB, P3 = 200 KB, P4 = 250 KB\n");
    printf("########################################################################################\n\n");
    simulate_first_fit(blocks_case2, 4, procs_case2, 4, "CASE 2 (UNEQUAL SIZES)");
    simulate_best_fit(blocks_case2, 4, procs_case2, 4, "CASE 2 (UNEQUAL SIZES)");

    return 0;
}
