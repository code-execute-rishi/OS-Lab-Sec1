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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PARTITIONS 50

typedef struct {
    int start_addr;
    int size;
    bool is_hole;
    char proc_name[10];
} Segment;

typedef struct {
    Segment segments[MAX_PARTITIONS];
    int count;
    int total_ram;
} MemoryState;

void init_memory(MemoryState *mem, int total_ram) {
    mem->total_ram = total_ram;
    mem->segments[0].start_addr = 0;
    mem->segments[0].size = total_ram;
    mem->segments[0].is_hole = true;
    strcpy(mem->segments[0].proc_name, "FREE");
    mem->count = 1;
}

// Coalesce contiguous free holes
void coalesce_holes(MemoryState *mem) {
    for (int i = 0; i < mem->count - 1; i++) {
        if (mem->segments[i].is_hole && mem->segments[i + 1].is_hole) {
            mem->segments[i].size += mem->segments[i + 1].size;
            // Shift remaining
            for (int k = i + 1; k < mem->count - 1; k++) {
                mem->segments[k] = mem->segments[k + 1];
            }
            mem->count--;
            i--; // re-check at same index
        }
    }
}

// Allocate using Worst Fit (largest available hole)
bool allocate_worst_fit(MemoryState *mem, const char *proc_name, int req_size) {
    int worst_idx = -1;
    int max_hole_size = -1;

    for (int i = 0; i < mem->count; i++) {
        if (mem->segments[i].is_hole && mem->segments[i].size >= req_size) {
            if (mem->segments[i].size > max_hole_size) {
                max_hole_size = mem->segments[i].size;
                worst_idx = i;
            }
        }
    }

    if (worst_idx == -1) {
        return false; // Cannot allocate
    }

    // If exact match
    if (mem->segments[worst_idx].size == req_size) {
        mem->segments[worst_idx].is_hole = false;
        strcpy(mem->segments[worst_idx].proc_name, proc_name);
    } else {
        // Split hole: segment[worst_idx] becomes process, new hole inserted after
        int remaining_hole = mem->segments[worst_idx].size - req_size;
        int orig_start = mem->segments[worst_idx].start_addr;

        // Shift right
        for (int k = mem->count; k > worst_idx + 1; k--) {
            mem->segments[k] = mem->segments[k - 1];
        }

        // Process segment
        mem->segments[worst_idx].start_addr = orig_start;
        mem->segments[worst_idx].size = req_size;
        mem->segments[worst_idx].is_hole = false;
        strcpy(mem->segments[worst_idx].proc_name, proc_name);

        // Remaining hole segment
        mem->segments[worst_idx + 1].start_addr = orig_start + req_size;
        mem->segments[worst_idx + 1].size = remaining_hole;
        mem->segments[worst_idx + 1].is_hole = true;
        strcpy(mem->segments[worst_idx + 1].proc_name, "FREE");

        mem->count++;
    }
    return true;
}

// Deallocate process
bool deallocate_process(MemoryState *mem, const char *proc_name) {
    for (int i = 0; i < mem->count; i++) {
        if (!mem->segments[i].is_hole && strcmp(mem->segments[i].proc_name, proc_name) == 0) {
            mem->segments[i].is_hole = true;
            strcpy(mem->segments[i].proc_name, "FREE");
            coalesce_holes(mem);
            return true;
        }
    }
    return false;
}

// Compute total free memory and external fragmentation for a pending request
int compute_total_free(const MemoryState *mem) {
    int total_free = 0;
    for (int i = 0; i < mem->count; i++) {
        if (mem->segments[i].is_hole) {
            total_free += mem->segments[i].size;
        }
    }
    return total_free;
}

int compute_external_fragmentation(const MemoryState *mem, int failed_req_size) {
    int total_free = compute_total_free(mem);
    if (total_free >= failed_req_size) {
        return total_free; // Fragmented free memory cannot satisfy contiguous request
    }
    return 0;
}

void print_memory_map(const MemoryState *mem, const char *event_title) {
    printf("  >>> Event: %-32s <<<\n", event_title);
    printf("  Memory Layout Map: [");
    int occupied_total = 0;
    int free_holes = 0;
    for (int i = 0; i < mem->count; i++) {
        if (mem->segments[i].is_hole) {
            free_holes++;
            printf("| FREE:%dKB (%d-%d) |",
                   mem->segments[i].size,
                   mem->segments[i].start_addr,
                   mem->segments[i].start_addr + mem->segments[i].size);
        } else {
            occupied_total += mem->segments[i].size;
            printf("| %s:%dKB (%d-%d) |",
                   mem->segments[i].proc_name,
                   mem->segments[i].size,
                   mem->segments[i].start_addr,
                   mem->segments[i].start_addr + mem->segments[i].size);
        }
    }
    printf("]\n");
    printf("  Occupied: %d KB | Available: %d KB in %d free hole(s)\n\n",
           occupied_total, mem->total_ram - occupied_total, free_holes);
}

void run_case1() {
    printf("########################################################################################\n");
    printf("  EXPERIMENT 11 - CASE 1: SIMULTANEOUS ARRIVAL AT TIME T\n");
    printf("  Total RAM: 1024 KB | Processes: P1(100KB), P2(200KB), P3(300KB), P4(300KB), P5(300KB)\n");
    printf("########################################################################################\n\n");

    MemoryState mem;
    init_memory(&mem, 1024);
    print_memory_map(&mem, "Initial State (Single 1024 KB Hole)");

    const char *procs[] = {"P1", "P2", "P3", "P4", "P5"};
    int sizes[] = {100, 200, 300, 300, 300};

    for (int i = 0; i < 5; i++) {
        char event_desc[64];
        sprintf(event_desc, "Arrival of %s (%d KB)", procs[i], sizes[i]);
        bool ok = allocate_worst_fit(&mem, procs[i], sizes[i]);
        if (ok) {
            print_memory_map(&mem, event_desc);
        } else {
            printf("  [ALERT] Failed to allocate %s (%d KB) under Worst Fit!\n", procs[i], sizes[i]);
            int ext_frag = compute_external_fragmentation(&mem, sizes[i]);
            printf("  Total Free Memory: %d KB | Required: %d KB | External Fragmentation: %d KB\n\n",
                   compute_total_free(&mem), sizes[i], ext_frag);
        }
    }
}

void run_case2() {
    printf("########################################################################################\n");
    printf("  EXPERIMENT 11 - CASE 2: TIMELY ORDERED ARRIVALS AND DEPARTURES (T1 TO T8)\n");
    printf("  Total RAM: 1024 KB | Demonstrating Dynamic Fragmentation and Hole Creation\n");
    printf("########################################################################################\n\n");

    MemoryState mem;
    init_memory(&mem, 1024);
    print_memory_map(&mem, "Initial State: 1024 KB Free Hole");

    struct Event {
        const char *time_step;
        const char *type; // "ARRIVE" or "LEAVE"
        const char *proc;
        int size;
    } events[] = {
        {"T1", "ARRIVE", "P1", 100},
        {"T2", "ARRIVE", "P2", 100},
        {"T3", "ARRIVE", "P3", 200},
        {"T4", "LEAVE",  "P2", 100},
        {"T5", "ARRIVE", "P4", 200},
        {"T6", "ARRIVE", "P5", 200},
        {"T7", "LEAVE",  "P4", 200},
        {"T8", "ARRIVE", "P6", 300}
    };

    int n_events = sizeof(events) / sizeof(events[0]);

    for (int i = 0; i < n_events; i++) {
        char desc[80];
        if (strcmp(events[i].type, "ARRIVE") == 0) {
            sprintf(desc, "%s: %s arrives (%d KB)", events[i].time_step, events[i].proc, events[i].size);
            bool ok = allocate_worst_fit(&mem, events[i].proc, events[i].size);
            if (ok) {
                print_memory_map(&mem, desc);
            } else {
                printf("  >>> Event: %-32s <<<\n", desc);
                printf("  [FAILED ALLOCATION] No single contiguous hole >= %d KB!\n", events[i].size);
                int total_free = compute_total_free(&mem);
                int ext_frag = compute_external_fragmentation(&mem, events[i].size);
                printf("  Total Free Memory Available = %d KB\n", total_free);
                printf("  Process Size Required       = %d KB\n", events[i].size);
                printf("  External Fragmentation      = %d KB\n", ext_frag);
                printf("  -> NOTE: Free space (%d KB) exceeds required (%d KB), but is non-contiguous!\n\n",
                       total_free, events[i].size);
            }
        } else {
            sprintf(desc, "%s: %s terminates/leaves (%d KB)", events[i].time_step, events[i].proc, events[i].size);
            deallocate_process(&mem, events[i].proc);
            print_memory_map(&mem, desc);
        }
    }
}

int main() {
    printf("========================================================================================\n");
    printf("   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 11\n");
    printf("   VARIABLE PARTITIONING MEMORY PLACEMENT: WORST FIT SIMULATION\n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("========================================================================================\n\n");

    run_case1();
    run_case2();

    return 0;
}
