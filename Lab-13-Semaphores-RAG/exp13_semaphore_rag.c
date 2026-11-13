/*
 * ==============================================================================
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 13
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * Semester       : V Semester, B.Tech CSE
 * Institution    : Maulana Azad National Institute of Technology (MANIT) Bhopal
 * ==============================================================================
 * AIM: Implementation of Binary & Counting Semaphores and
 *      Cycle Detection in Resource Allocation Graph (RAG) for Deadlock Identification.
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

// -----------------------------------------------------------------------------
// PART A: SEMAPHORE IMPLEMENTATION (BINARY & COUNTING SEMAPHORES)
// -----------------------------------------------------------------------------

#define ITERATIONS 100000

volatile long shared_counter_unsafe = 0;
volatile long shared_counter_safe = 0;

sem_t *bin_sem;

void* thread_unsafe(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        long temp = shared_counter_unsafe;
        usleep(0); // force voluntary yield to expose race condition
        shared_counter_unsafe = temp + 1;
    }
    return NULL;
}

void* thread_binary_sem(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(bin_sem); // Critical Section Lock (P operation)
        long temp = shared_counter_safe;
        shared_counter_safe = temp + 1;
        sem_post(bin_sem); // Release Lock (V operation)
    }
    return NULL;
}

// Counting Semaphore demonstration: Pool of 2 printers shared among 4 processes
sem_t *cnt_sem;

void* worker_counting_sem(void *arg) {
    long id = (long)arg;
    printf("  [Process P%ld] Requesting resource from pool (waiting on counting semaphore)...\n", id);
    sem_wait(cnt_sem);

    printf("  >>> [Process P%ld] ACQUIRED RESOURCE! Accessing critical shared resource unit...\n", id);
    usleep(50000); // 50 ms work
    printf("  <<< [Process P%ld] FINISHED! Releasing resource back to pool.\n", id);

    sem_post(cnt_sem);
    return NULL;
}

void run_semaphore_demo() {
    printf("========================================================================================\n");
    printf("  PART A: SEMAPHORE IMPLEMENTATION (BINARY & COUNTING SEMAPHORES)\n");
    printf("========================================================================================\n\n");

    printf("1. Binary Semaphore Demonstration (Mutual Exclusion on Shared Variable):\n");
    printf("----------------------------------------------------------------------------------------\n");

    // Create named semaphore for maximum cross-platform compatibility (macOS & Linux)
    sem_unlink("/bin_sem_manit");
    bin_sem = sem_open("/bin_sem_manit", O_CREAT | O_EXCL, 0644, 1);
    if (bin_sem == SEM_FAILED) {
        perror("sem_open failed");
        return;
    }

    pthread_t t1, t2;

    // Unsafe Execution (Race Condition)
    pthread_create(&t1, NULL, thread_unsafe, NULL);
    pthread_create(&t2, NULL, thread_unsafe, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Safe Execution using Binary Semaphore
    pthread_create(&t1, NULL, thread_binary_sem, NULL);
    pthread_create(&t2, NULL, thread_binary_sem, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_close(bin_sem);
    sem_unlink("/bin_sem_manit");

    printf("  Expected Final Value (2 threads x %d)  : %d\n", ITERATIONS, 2 * ITERATIONS);
    printf("  Result WITHOUT Semaphore (Race Hazard)        : %ld (DATA INCONSISTENCY)\n", shared_counter_unsafe);
    printf("  Result WITH Binary Semaphore (Mutual Exclusion) : %ld (SYNCHRONIZED - EXACT)\n\n", shared_counter_safe);

    // 2. Counting Semaphore
    printf("2. Counting Semaphore Demonstration (Resource Pool of 2 Instances shared by 4 Tasks):\n");
    printf("----------------------------------------------------------------------------------------\n");
    sem_unlink("/cnt_sem_manit");
    cnt_sem = sem_open("/cnt_sem_manit", O_CREAT | O_EXCL, 0644, 2); // 2 instances

    pthread_t workers[4];
    for (long i = 1; i <= 4; i++) {
        pthread_create(&workers[i - 1], NULL, worker_counting_sem, (void*)i);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(workers[i], NULL);
    }
    sem_close(cnt_sem);
    sem_unlink("/cnt_sem_manit");
    printf("========================================================================================\n\n");
}

// -----------------------------------------------------------------------------
// PART B: RESOURCE ALLOCATION GRAPH (RAG) & CYCLE DETECTION PROCEDURE
// -----------------------------------------------------------------------------

#define MAX_NODES 40

typedef struct {
    int num_processes;
    int num_resources;
    int total_nodes; // Processes are 0 to m-1, Resources are m to m+n-1
    int adj[MAX_NODES][MAX_NODES];
    char node_names[MAX_NODES][10];
} RAG;

void init_rag(RAG *rag, int m, int n) {
    rag->num_processes = m;
    rag->num_resources = n;
    rag->total_nodes = m + n;

    for (int i = 0; i < rag->total_nodes; i++) {
        for (int j = 0; j < rag->total_nodes; j++) {
            rag->adj[i][j] = 0;
        }
    }

    for (int i = 0; i < m; i++) sprintf(rag->node_names[i], "P%d", i + 1);
    for (int j = 0; j < n; j++) sprintf(rag->node_names[m + j], "R%d", j + 1);
}

void add_request_edge(RAG *rag, int p_idx, int r_idx) {
    rag->adj[p_idx][rag->num_processes + r_idx] = 1;
}

void add_assignment_edge(RAG *rag, int r_idx, int p_idx) {
    rag->adj[rag->num_processes + r_idx][p_idx] = 1;
}

bool dfs_cycle(const RAG *rag, int u, int color[], int parent[], int *cycle_start, int *cycle_end) {
    color[u] = 1; // Mark Gray (currently in recursion call stack)

    for (int v = 0; v < rag->total_nodes; v++) {
        if (rag->adj[u][v]) {
            if (color[v] == 1) { // Back edge found! Directed cycle detected!
                *cycle_start = v;
                *cycle_end = u;
                return true;
            }
            if (color[v] == 0) {
                parent[v] = u;
                if (dfs_cycle(rag, v, color, parent, cycle_start, cycle_end)) {
                    return true;
                }
            }
        }
    }

    color[u] = 2; // Mark Black (fully explored)
    return false;
}

void analyze_rag(const RAG *rag, const char *scenario_name) {
    printf(">>> SCENARIO: %s <<<\n", scenario_name);
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Processes: %d | Single-Instance Resources: %d\n", rag->num_processes, rag->num_resources);
    printf("  Graph Directed Edges:\n");

    int edge_count = 0;
    for (int u = 0; u < rag->total_nodes; u++) {
        for (int v = 0; v < rag->total_nodes; v++) {
            if (rag->adj[u][v]) {
                if (u < rag->num_processes) {
                    printf("    Request Edge    : %s ----> %s (Process requests Resource)\n",
                           rag->node_names[u], rag->node_names[v]);
                } else {
                    printf("    Assignment Edge : %s ----> %s (Resource allocated to Process)\n",
                           rag->node_names[u], rag->node_names[v]);
                }
                edge_count++;
            }
        }
    }
    if (edge_count == 0) printf("    (No edges present)\n");

    int color[MAX_NODES] = {0};
    int parent[MAX_NODES];
    for (int i = 0; i < MAX_NODES; i++) parent[i] = -1;

    int cycle_start = -1, cycle_end = -1;
    bool has_cycle = false;

    for (int u = 0; u < rag->total_nodes; u++) {
        if (color[u] == 0) {
            if (dfs_cycle(rag, u, color, parent, &cycle_start, &cycle_end)) {
                has_cycle = true;
                break;
            }
        }
    }

    if (has_cycle) {
        printf("\n  >>> STATUS: DEADLOCK DETECTED! <<<\n");
        printf("  Circular Wait Condition satisfied in single-instance Resource Allocation Graph.\n");
        printf("  Directed Cycle Identified: ");

        int path[MAX_NODES];
        int path_len = 0;
        path[path_len++] = cycle_start;
        for (int curr = cycle_end; curr != cycle_start && curr != -1; curr = parent[curr]) {
            path[path_len++] = curr;
        }
        path[path_len++] = cycle_start;

        for (int i = path_len - 1; i >= 0; i--) {
            printf("%s%s", rag->node_names[path[i]], (i == 0) ? "" : " -> ");
        }
        printf("\n");
        printf("  Processes deadlocked: ");
        for (int i = path_len - 1; i >= 1; i--) { /* path[0] repeats the cycle start */
            if (path[i] < rag->num_processes) {
                printf("%s ", rag->node_names[path[i]]);
            }
        }
        printf("\n\n");
    } else {
        printf("\n  >>> STATUS: DEADLOCK FREE <<<\n");
        printf("  No directed cycles detected. System is safe and deadlock-free.\n\n");
    }
}

void run_rag_demo() {
    printf("========================================================================================\n");
    printf("  PART B: RESOURCE ALLOCATION GRAPH (RAG) & CYCLE DETECTION\n");
    printf("========================================================================================\n\n");

    // Case 1: Deadlock Free System
    // P1 -> R1, R1 -> P2, P2 -> R2, R2 -> P3
    RAG rag1;
    init_rag(&rag1, 3, 2);
    add_request_edge(&rag1, 0, 0);    // P1 -> R1
    add_assignment_edge(&rag1, 0, 1); // R1 -> P2
    add_request_edge(&rag1, 1, 1);    // P2 -> R2
    add_assignment_edge(&rag1, 1, 2); // R2 -> P3
    analyze_rag(&rag1, "CASE 1 - ACYCLIC GRAPH (DEADLOCK FREE)");

    // Case 2: Deadlock Condition with Cycle
    // P1 holds R2, requests R1; P2 holds R1, requests R2
    // Edges: R2 -> P1, P1 -> R1, R1 -> P2, P2 -> R2
    RAG rag2;
    init_rag(&rag2, 2, 2);
    add_assignment_edge(&rag2, 1, 0); // R2 -> P1
    add_request_edge(&rag2, 0, 0);    // P1 -> R1
    add_assignment_edge(&rag2, 0, 1); // R1 -> P2
    add_request_edge(&rag2, 1, 1);    // P2 -> R2
    analyze_rag(&rag2, "CASE 2 - CIRCULAR DEPENDENCY (DEADLOCK CYCLE)");
}

int main() {
    run_semaphore_demo();
    run_rag_demo();
    return 0;
}
