/**
 * ============================================================================
 *   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 6
 *   Student Name   : Aradhya Jain
 *   Scholar Number : 24112011111
 *   Section        : CSE-1
 *   Semester       : V Semester, B.Tech CSE
 *   Institution    : Maulana Azad National Institute of Technology (MANIT)
 * ============================================================================
 *
 * a. Generic CPU scheduling: FCFS, SJF, SRTF, Round Robin,
 *    Priority (Non-Preemptive), Priority (Preemptive).
 *    Input: n, then "arrival burst priority" per process, then algorithm.
 *    Output: CT, TAT, WT, RT per process and their averages.
 * b. Round Robin on fixed data P1(0,10) P2(1,4) P3(3,6) P4(4,7) P5(5,2)
 *    for q = 1..10, and find the best q for TAT, WT and RT.
 *
 * Formulas: TAT = CT - AT,  WT = TAT - BT,  RT = first CPU time - AT
 * Lower priority number = higher priority.
 */

#include <stdio.h>
#include <string.h>

#define MAX 20

typedef struct {
    int pid, at, bt, pr;   /* input: id, arrival, burst, priority */
    int ct, rt;            /* output: completion, response */
    int rem, done;         /* work: remaining burst, finished flag */
} Process;

void print_table(Process p[], int n, const char *name) {
    float total_tat = 0, total_wt = 0, total_rt = 0;
    printf("\n========================================================================================\n");
    printf("  CPU SCHEDULING ALGORITHM: %s\n", name);
    printf("========================================================================================\n");
    printf("%-8s %-12s %-12s %-10s %-8s %-8s %-8s %-8s\n",
           "PID", "Arrival(AT)", "Burst(BT)", "Priority", "CT", "TAT", "WT", "RT");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        int tat = p[i].ct - p[i].at;
        int wt = tat - p[i].bt;
        total_tat += tat;
        total_wt += wt;
        total_rt += p[i].rt;
        printf("P%-7d %-12d %-12d %-10d %-8d %-8d %-8d %-8d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr, p[i].ct, tat, wt, p[i].rt);
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Average Turnaround Time (TAT) : %.2f ms\n", total_tat / n);
    printf("Average Waiting Time (WT)    : %.2f ms\n", total_wt / n);
    printf("Average Response Time (RT)   : %.2f ms\n", total_rt / n);
    printf("========================================================================================\n\n");
}

/* FCFS: sort by arrival, then run one after another */
void fcfs(Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].at > p[j + 1].at) {
                Process t = p[j]; p[j] = p[j + 1]; p[j + 1] = t;
            }

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;   /* CPU idle until it arrives */
        p[i].rt = time - p[i].at;
        time += p[i].bt;
        p[i].ct = time;
    }
    print_table(p, n, "First Come First Serve (FCFS)");
}

/* SJF / Priority (Non-Preemptive): pick arrived job with smallest key, run it fully */
void non_preemptive(Process p[], int n, int by_priority, const char *name) {
    for (int i = 0; i < n; i++) p[i].done = 0;

    int time = 0, finished = 0;
    while (finished < n) {
        int best = -1, min = 1e9;
        for (int i = 0; i < n; i++) {
            int key = by_priority ? p[i].pr : p[i].bt;
            if (!p[i].done && p[i].at <= time && key < min) { min = key; best = i; }
        }
        if (best == -1) { time++; continue; }   /* nobody arrived: CPU idle */

        p[best].rt = time - p[best].at;
        time += p[best].bt;
        p[best].ct = time;
        p[best].done = 1;
        finished++;
    }
    print_table(p, n, name);
}

/* SRTF / Priority (Preemptive): every 1 ms pick arrived job with smallest key, run 1 ms */
void preemptive(Process p[], int n, int by_priority, const char *name) {
    for (int i = 0; i < n; i++) p[i].rem = p[i].bt;

    int time = 0, finished = 0;
    while (finished < n) {
        int best = -1, min = 1e9;
        for (int i = 0; i < n; i++) {
            int key = by_priority ? p[i].pr : p[i].rem;
            if (p[i].rem > 0 && p[i].at <= time && key < min) { min = key; best = i; }
        }
        if (best == -1) { time++; continue; }   /* nobody arrived: CPU idle */

        if (p[best].rem == p[best].bt) p[best].rt = time - p[best].at;   /* first run */
        p[best].rem--;
        time++;
        if (p[best].rem == 0) { p[best].ct = time; finished++; }
    }
    print_table(p, n, name);
}

/* Round Robin: returns number of context switches (CPU dispatches) */
int round_robin(Process p[], int n, int q) {
    int queue[2000], front = 0, rear = 0;
    int added[MAX] = {0}, time = 0, finished = 0, switches = 0, prev = -1;
    for (int i = 0; i < n; i++) p[i].rem = p[i].bt;

    while (finished < n) {
        /* new arrivals join the queue first, then the preempted process */
        for (int i = 0; i < n; i++)
            if (!added[i] && p[i].at <= time) { queue[rear++] = i; added[i] = 1; }
        if (prev != -1) { queue[rear++] = prev; prev = -1; }

        if (front == rear) { time++; continue; }   /* queue empty: CPU idle */

        int i = queue[front++];
        switches++;
        if (p[i].rem == p[i].bt) p[i].rt = time - p[i].at;   /* first run */
        int run = p[i].rem < q ? p[i].rem : q;
        p[i].rem -= run;
        time += run;
        if (p[i].rem > 0) prev = i;
        else { p[i].ct = time; finished++; }
    }
    return switches;
}

void run_lab6_part_a(void) {
    printf("\n======================================================================\n");
    printf("  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C\n");
    printf("  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("======================================================================\n");

    int n;
    printf("Enter number of processes (e.g. 5): ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX) {
        printf("Invalid input or process count too high.\n");
        return;
    }

    Process p[MAX];
    printf("\nEnter process details in sequence (PID, Arrival Time, Burst Time, Priority):\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Process P%d -> Arrival Time, Burst Time, Priority: ", i + 1);
        if (scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr) != 3) {
            printf("Error reading input.\n");
            return;
        }
    }

    printf("\nSelect CPU Scheduling Algorithm:\n");
    printf("  1. FCFS (First Come First Serve)\n");
    printf("  2. SJF (Shortest Job First - Non-Preemptive)\n");
    printf("  3. SRTF (Shortest Remaining Time First - Preemptive SJF)\n");
    printf("  4. Round Robin (RR)\n");
    printf("  5. Priority (Non-Preemptive)\n");
    printf("  6. Priority (Preemptive)\n");
    printf("Enter selection [1-6]: ");

    int choice;
    if (scanf("%d", &choice) != 1) return;

    if (choice == 1) fcfs(p, n);
    else if (choice == 2) non_preemptive(p, n, 0, "Shortest Job First (SJF - Non-Preemptive)");
    else if (choice == 3) preemptive(p, n, 0, "Shortest Remaining Time First (SRTF - Preemptive)");
    else if (choice == 4) {
        int q;
        printf("Enter Time Quantum (ms): ");
        if (scanf("%d", &q) != 1 || q <= 0) q = 2;
        round_robin(p, n, q);
        char name[64];
        snprintf(name, sizeof(name), "Round Robin (RR, Quantum = %d ms)", q);
        print_table(p, n, name);
    }
    else if (choice == 5) non_preemptive(p, n, 1, "Priority Scheduling (Non-Preemptive)");
    else if (choice == 6) preemptive(p, n, 1, "Priority Scheduling (Preemptive)");
    else printf("Invalid algorithm selection.\n");
}

void run_lab6_part_b(void) {
    printf("\n======================================================================\n");
    printf("  OS LAB 6 (Part b): Round Robin Parametric Time Quantum Sweep (q=1..10)\n");
    printf("  Benchmark: P1(0,10), P2(1,4), P3(3,6), P4(4,7), P5(5,2)\n");
    printf("  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("======================================================================\n");

    int n = 5, at[5] = {0, 1, 3, 4, 5}, bt[5] = {10, 4, 6, 7, 2};
    Process p[5];
    for (int i = 0; i < n; i++) { p[i].pid = i + 1; p[i].at = at[i]; p[i].bt = bt[i]; }

    printf("\n%-14s %-16s %-16s %-16s %-12s\n",
           "Quantum (q)", "Avg TAT (ms)", "Avg WT (ms)", "Avg RT (ms)", "Context Switches");
    printf("--------------------------------------------------------------------------------\n");

    float min_tat = 1e9, min_wt = 1e9, min_rt = 1e9;
    int best_tat = 1, best_wt = 1, best_rt = 1;

    for (int q = 1; q <= 10; q++) {
        int switches = round_robin(p, n, q);

        float total_tat = 0, total_wt = 0, total_rt = 0;
        for (int i = 0; i < n; i++) {
            int tat = p[i].ct - p[i].at;
            total_tat += tat;
            total_wt += tat - p[i].bt;
            total_rt += p[i].rt;
        }
        float avg_tat = total_tat / n, avg_wt = total_wt / n, avg_rt = total_rt / n;

        printf("q = %-9d %-16.2f %-16.2f %-16.2f %-12d\n",
               q, avg_tat, avg_wt, avg_rt, switches);

        /* keep the first q that gives the smallest average */
        if (avg_tat < min_tat) { min_tat = avg_tat; best_tat = q; }
        if (avg_wt < min_wt)   { min_wt = avg_wt;   best_wt = q; }
        if (avg_rt < min_rt)   { min_rt = avg_rt;   best_rt = q; }
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("[OPTIMAL ANALYSIS RESULTS]:\n");
    printf("  Optimal Quantum for Turnaround Time (TAT) : q = %d ms (Avg TAT = %.2f ms)\n", best_tat, min_tat);
    printf("  Optimal Quantum for Waiting Time (WT)    : q = %d ms (Avg WT  = %.2f ms)\n", best_wt, min_wt);
    printf("  Optimal Quantum for Response Time (RT)   : q = %d ms (Avg RT  = %.2f ms)\n", best_rt, min_rt);
    printf("======================================================================\n\n");
}

int main(int argc, char *argv[]) {
    if (argc >= 2 && strcmp(argv[1], "--lab6a") == 0) { run_lab6_part_a(); return 0; }
    if (argc >= 2 && strcmp(argv[1], "--lab6b") == 0) { run_lab6_part_b(); return 0; }

    while (1) {
        printf("======================================================================\n");
        printf("  OPERATING SYSTEMS LAB (CS-502) - EXPERIMENT 6\n");
        printf("  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
        printf("======================================================================\n");
        printf("Select an experiment to execute:\n");
        printf("  1. Lab 6 (a) : Generic CPU Scheduling Algorithms (Interactive)\n");
        printf("  2. Lab 6 (b) : Round Robin Benchmark & Quantum Sweep (q = 1..10 ms)\n");
        printf("  0. Exit\n");
        printf("Enter selection [0-2]: ");

        int opt;
        if (scanf("%d", &opt) != 1) break;

        if (opt == 1) run_lab6_part_a();
        else if (opt == 2) run_lab6_part_b();
        else if (opt == 0) { printf("Exiting. Thank you!\n"); break; }
        else printf("Invalid selection.\n\n");
    }
    return 0;
}
