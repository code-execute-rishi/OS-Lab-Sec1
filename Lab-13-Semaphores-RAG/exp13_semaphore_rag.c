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
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

// ---------------- PART A: SEMAPHORES ----------------

#define ITERATIONS 100000

long unsafeCount = 0, safeCount = 0;
sem_t *binSem, *cntSem;   // named semaphores (sem_open works on macOS & Linux)

void *withoutSem(void *arg) {
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++) {
        long temp = unsafeCount;
        usleep(0);                 // yield between read and write -> race
        unsafeCount = temp + 1;
    }
    return NULL;
}

void *withSem(void *arg) {
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(binSem);          // P(): enter critical section
        safeCount++;
        sem_post(binSem);          // V(): leave critical section
    }
    return NULL;
}

void *worker(void *arg) {
    long id = (long)arg;
    printf("  [Process P%ld] Requesting resource from pool (waiting on counting semaphore)...\n", id);
    sem_wait(cntSem);              // only 2 workers get past at a time
    printf("  >>> [Process P%ld] ACQUIRED RESOURCE! Accessing critical shared resource unit...\n", id);
    usleep(50000);
    printf("  <<< [Process P%ld] FINISHED! Releasing resource back to pool.\n", id);
    sem_post(cntSem);
    return NULL;
}

void semaphores() {
    printf("========================================================================================\n");
    printf("  PART A: SEMAPHORE IMPLEMENTATION (BINARY & COUNTING SEMAPHORES)\n");
    printf("========================================================================================\n\n");

    printf("1. Binary Semaphore Demonstration (Mutual Exclusion on Shared Variable):\n");
    printf("----------------------------------------------------------------------------------------\n");
    sem_unlink("/bin_sem_manit");
    binSem = sem_open("/bin_sem_manit", O_CREAT, 0644, 1);   // binary: initial value 1

    pthread_t t1, t2;
    pthread_create(&t1, NULL, withoutSem, NULL);
    pthread_create(&t2, NULL, withoutSem, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_create(&t1, NULL, withSem, NULL);
    pthread_create(&t2, NULL, withSem, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_close(binSem);
    sem_unlink("/bin_sem_manit");

    printf("  Expected Final Value (2 threads x %d)  : %d\n", ITERATIONS, 2 * ITERATIONS);
    printf("  Result WITHOUT Semaphore (Race Hazard)        : %ld (DATA INCONSISTENCY)\n", unsafeCount);
    printf("  Result WITH Binary Semaphore (Mutual Exclusion) : %ld (SYNCHRONIZED - EXACT)\n\n", safeCount);

    printf("2. Counting Semaphore Demonstration (Resource Pool of 2 Instances shared by 4 Tasks):\n");
    printf("----------------------------------------------------------------------------------------\n");
    sem_unlink("/cnt_sem_manit");
    cntSem = sem_open("/cnt_sem_manit", O_CREAT, 0644, 2);   // counting: 2 instances

    pthread_t w[4];
    for (long i = 0; i < 4; i++) pthread_create(&w[i], NULL, worker, (void *)(i + 1));
    for (int i = 0; i < 4; i++) pthread_join(w[i], NULL);

    sem_close(cntSem);
    sem_unlink("/cnt_sem_manit");
    printf("========================================================================================\n\n");
}

// ---------------- PART B: RESOURCE ALLOCATION GRAPH ----------------
// Nodes 0..m-1 are processes, nodes m..m+n-1 are resources.

int m, n;
int adj[10][10];
char name[10][4];
int color[10];    // 0 = white (unvisited), 1 = gray (on stack), 2 = black (done)
int parent[10];
int cycStart, cycEnd;

void setup(int procs, int res) {
    m = procs;
    n = res;
    for (int i = 0; i < m + n; i++)
        for (int j = 0; j < m + n; j++) adj[i][j] = 0;
    for (int i = 0; i < m; i++) sprintf(name[i], "P%d", i + 1);
    for (int i = 0; i < n; i++) sprintf(name[m + i], "R%d", i + 1);
}

// DFS: reaching a gray node again means a cycle
int dfs(int u) {
    color[u] = 1;
    for (int v = 0; v < m + n; v++) {
        if (!adj[u][v]) continue;
        if (color[v] == 1) {
            cycStart = v;
            cycEnd = u;
            return 1;
        }
        if (color[v] == 0) {
            parent[v] = u;
            if (dfs(v)) return 1;
        }
    }
    color[u] = 2;
    return 0;
}

void analyze(const char *title) {
    printf(">>> SCENARIO: %s <<<\n", title);
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Processes: %d | Single-Instance Resources: %d\n", m, n);
    printf("  Graph Directed Edges:\n");
    for (int u = 0; u < m + n; u++)
        for (int v = 0; v < m + n; v++)
            if (adj[u][v]) {
                if (u < m)
                    printf("    Request Edge    : %s ----> %s (Process requests Resource)\n", name[u], name[v]);
                else
                    printf("    Assignment Edge : %s ----> %s (Resource allocated to Process)\n", name[u], name[v]);
            }

    int found = 0;
    for (int i = 0; i < m + n; i++) color[i] = 0;
    for (int i = 0; i < m + n && !found; i++)
        if (color[i] == 0) found = dfs(i);

    if (!found) {
        printf("\n  >>> STATUS: DEADLOCK FREE <<<\n");
        printf("  No directed cycles detected. System is safe and deadlock-free.\n\n");
        return;
    }

    printf("\n  >>> STATUS: DEADLOCK DETECTED! <<<\n");
    printf("  Circular Wait Condition satisfied in single-instance Resource Allocation Graph.\n");

    // walk parent[] back from cycEnd to cycStart (path comes out reversed)
    int path[10], len = 0;
    for (int v = cycEnd; v != cycStart; v = parent[v]) path[len++] = v;
    path[len++] = cycStart;

    printf("  Directed Cycle Identified: ");
    for (int i = len - 1; i >= 0; i--) printf("%s -> ", name[path[i]]);
    printf("%s\n", name[cycStart]);

    printf("  Processes deadlocked: ");
    for (int i = len - 1; i >= 0; i--)
        if (path[i] < m) printf("%s ", name[path[i]]);
    printf("\n\n");
}

void rag() {
    printf("========================================================================================\n");
    printf("  PART B: RESOURCE ALLOCATION GRAPH (RAG) & CYCLE DETECTION\n");
    printf("========================================================================================\n\n");

    // Case 1: P1=0 P2=1 P3=2 R1=3 R2=4
    setup(3, 2);
    adj[0][3] = 1;   // P1 -> R1
    adj[3][1] = 1;   // R1 -> P2
    adj[1][4] = 1;   // P2 -> R2
    adj[4][2] = 1;   // R2 -> P3
    analyze("CASE 1 - ACYCLIC GRAPH (DEADLOCK FREE)");

    // Case 2: P1=0 P2=1 R1=2 R2=3
    setup(2, 2);
    adj[3][0] = 1;   // R2 -> P1
    adj[0][2] = 1;   // P1 -> R1
    adj[2][1] = 1;   // R1 -> P2
    adj[1][3] = 1;   // P2 -> R2
    analyze("CASE 2 - CIRCULAR DEPENDENCY (DEADLOCK CYCLE)");
}

int main() {
    semaphores();
    rag();
    return 0;
}
