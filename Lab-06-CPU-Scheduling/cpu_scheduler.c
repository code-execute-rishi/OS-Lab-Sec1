/**
 * ============================================================================
 *   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 6
 *   Student Name   : Aradhya Jain
 *   Scholar Number : 24112011111
 *   Section        : CSE-1
 *   Semester       : V Semester, B.Tech CSE
 *   Institution    : Maulana Azad National Institute of Technology (MANIT)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define MAX_PROC 20


/* ============================================================================
 * QUESTION / EXPERIMENT 6: CPU SCHEDULING ALGORITHMS
 * ============================================================================
 * a. Write generic code in C of CPU Scheduling Algorithms:
 *    - FCFS (First Come First Serve)
 *    - SJF (Shortest Job First - Non-Preemptive)
 *    - SRTF (Shortest Remaining Time First - Preemptive SJF)
 *    - RR (Round Robin)
 *    - Non-Preemptive Priority
 *    - Preemptive Priority
 *    In the program first user will input data in sequence like process no.,
 *    arrival time, burst time, and priority and then select any one algorithm
 *    to get TAT, WT and RT for each process and their averages. (If RR is selected,
 *    also take time quantum as input).
 *
 * b. Write generic simulation code in C for RR algorithm for given data:
 *      Process   Arrival Time   Burst Time
 *        P1            0             10
 *        P2            1              4
 *        P3            3              6
 *        P4            4              7
 *        P5            5              2
 *    Perform the analysis of different time quantum values (q = 1 to 10 ms)
 *    and find out the optimal value of time quantum for TAT, WT, and RT.
 * ============================================================================ */

typedef struct {
    int pid;
    int at;                  /* Arrival Time */
    int bt;                  /* Burst Time */
    int priority;            /* Priority (lower number = higher priority) */
    int ct;                  /* Completion Time */
    int tat;                 /* Turnaround Time = CT - AT */
    int wt;                  /* Waiting Time = TAT - BT */
    int rt;                  /* Response Time = First Response - AT */
    int rem_bt;              /* Remaining Burst Time for preemptive scheduling */
    int first_response_time; /* Time when CPU first touched process */
    bool completed;
} Process;

/* Helper: Print scheduling output table */
void print_schedule_table(Process p[], int n, const char *algo_name) {
    float total_tat = 0, total_wt = 0, total_rt = 0;
    printf("\n========================================================================================\n");
    printf("  CPU SCHEDULING ALGORITHM: %s\n", algo_name);
    printf("========================================================================================\n");
    printf("%-8s %-12s %-12s %-10s %-8s %-8s %-8s %-8s\n",
           "PID", "Arrival(AT)", "Burst(BT)", "Priority", "CT", "TAT", "WT", "RT");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        total_rt += p[i].rt;
        printf("P%-7d %-12d %-12d %-10d %-8d %-8d %-8d %-8d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].priority,
               p[i].ct, p[i].tat, p[i].wt, p[i].rt);
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("Average Turnaround Time (TAT) : %.2f ms\n", total_tat / n);
    printf("Average Waiting Time (WT)    : %.2f ms\n", total_wt / n);
    printf("Average Response Time (RT)   : %.2f ms\n", total_rt / n);
    printf("========================================================================================\n\n");
}

/* 1. First Come First Serve (FCFS) */
void schedule_fcfs(Process p[], int n) {
    Process temp[MAX_PROC];
    for (int i = 0; i < n; i++) temp[i] = p[i];

    /* Sort by arrival time */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp[j].at > temp[j + 1].at) {
                Process t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < temp[i].at) current_time = temp[i].at;
        temp[i].rt = current_time - temp[i].at;
        current_time += temp[i].bt;
        temp[i].ct = current_time;
        temp[i].tat = temp[i].ct - temp[i].at;
        temp[i].wt = temp[i].tat - temp[i].bt;
    }
    print_schedule_table(temp, n, "First Come First Serve (FCFS)");
}

/* 2. Shortest Job First (SJF - Non-Preemptive) */
void schedule_sjf_non_preemptive(Process p[], int n) {
    Process temp[MAX_PROC];
    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].completed = false;
    }

    int completed = 0, current_time = 0;
    while (completed < n) {
        int idx = -1;
        int min_bt = 1e9;
        for (int i = 0; i < n; i++) {
            if (temp[i].at <= current_time && !temp[i].completed) {
                if (temp[i].bt < min_bt) {
                    min_bt = temp[i].bt;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
        } else {
            temp[idx].rt = current_time - temp[idx].at;
            current_time += temp[idx].bt;
            temp[idx].ct = current_time;
            temp[idx].tat = temp[idx].ct - temp[idx].at;
            temp[idx].wt = temp[idx].tat - temp[idx].bt;
            temp[idx].completed = true;
            completed++;
        }
    }
    print_schedule_table(temp, n, "Shortest Job First (SJF - Non-Preemptive)");
}

/* 3. Shortest Remaining Time First (SRTF - Preemptive SJF) */
void schedule_srtf_preemptive(Process p[], int n) {
    Process temp[MAX_PROC];
    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].rem_bt = temp[i].bt;
        temp[i].first_response_time = -1;
        temp[i].completed = false;
    }

    int completed = 0, current_time = 0;
    while (completed < n) {
        int idx = -1;
        int min_rem = 1e9;

        for (int i = 0; i < n; i++) {
            if (temp[i].at <= current_time && !temp[i].completed) {
                if (temp[i].rem_bt < min_rem && temp[i].rem_bt > 0) {
                    min_rem = temp[i].rem_bt;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
        } else {
            if (temp[idx].first_response_time == -1) {
                temp[idx].first_response_time = current_time;
                temp[idx].rt = current_time - temp[idx].at;
            }

            temp[idx].rem_bt--;
            current_time++;

            if (temp[idx].rem_bt == 0) {
                temp[idx].ct = current_time;
                temp[idx].tat = temp[idx].ct - temp[idx].at;
                temp[idx].wt = temp[idx].tat - temp[idx].bt;
                temp[idx].completed = true;
                completed++;
            }
        }
    }
    print_schedule_table(temp, n, "Shortest Remaining Time First (SRTF - Preemptive)");
}

/* 4. Round Robin (RR) */
void schedule_round_robin(Process p[], int n, int quantum) {
    Process temp[MAX_PROC];
    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].rem_bt = temp[i].bt;
        temp[i].first_response_time = -1;
        temp[i].completed = false;
    }

    int completed = 0, current_time = 0;
    int queue[MAX_PROC * 100];
    int front = 0, rear = 0;
    bool in_queue[MAX_PROC] = {false};

    /* Enqueue processes arriving at time 0 */
    for (int i = 0; i < n; i++) {
        if (temp[i].at <= current_time) {
            queue[rear++] = i;
            in_queue[i] = true;
        }
    }

    while (completed < n) {
        if (front == rear) {
            /* Queue empty: advance time to next arrival */
            current_time++;
            for (int i = 0; i < n; i++) {
                if (temp[i].at <= current_time && !in_queue[i] && !temp[i].completed) {
                    queue[rear++] = i;
                    in_queue[i] = true;
                }
            }
            continue;
        }

        int idx = queue[front++];
        in_queue[idx] = false;

        if (temp[idx].first_response_time == -1) {
            temp[idx].first_response_time = current_time;
            temp[idx].rt = current_time - temp[idx].at;
        }

        int slice = (temp[idx].rem_bt > quantum) ? quantum : temp[idx].rem_bt;
        temp[idx].rem_bt -= slice;
        current_time += slice;

        /* Enqueue newly arrived processes during this slice */
        for (int i = 0; i < n; i++) {
            if (i != idx && temp[i].at <= current_time && !in_queue[i] && !temp[i].completed) {
                queue[rear++] = i;
                in_queue[i] = true;
            }
        }

        if (temp[idx].rem_bt > 0) {
            queue[rear++] = idx;
            in_queue[idx] = true;
        } else {
            temp[idx].ct = current_time;
            temp[idx].tat = temp[idx].ct - temp[idx].at;
            temp[idx].wt = temp[idx].tat - temp[idx].bt;
            temp[idx].completed = true;
            completed++;
        }
    }

    char title[64];
    snprintf(title, sizeof(title), "Round Robin (RR, Quantum = %d ms)", quantum);
    print_schedule_table(temp, n, title);
}

/* 5. Priority Scheduling (Non-Preemptive, Lower number = Higher priority) */
void schedule_priority_non_preemptive(Process p[], int n) {
    Process temp[MAX_PROC];
    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].completed = false;
    }

    int completed = 0, current_time = 0;
    while (completed < n) {
        int idx = -1;
        int best_prio = 1e9;

        for (int i = 0; i < n; i++) {
            if (temp[i].at <= current_time && !temp[i].completed) {
                if (temp[i].priority < best_prio) {
                    best_prio = temp[i].priority;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
        } else {
            temp[idx].rt = current_time - temp[idx].at;
            current_time += temp[idx].bt;
            temp[idx].ct = current_time;
            temp[idx].tat = temp[idx].ct - temp[idx].at;
            temp[idx].wt = temp[idx].tat - temp[idx].bt;
            temp[idx].completed = true;
            completed++;
        }
    }
    print_schedule_table(temp, n, "Priority Scheduling (Non-Preemptive)");
}

/* 6. Priority Scheduling (Preemptive) */
void schedule_priority_preemptive(Process p[], int n) {
    Process temp[MAX_PROC];
    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].rem_bt = temp[i].bt;
        temp[i].first_response_time = -1;
        temp[i].completed = false;
    }

    int completed = 0, current_time = 0;
    while (completed < n) {
        int idx = -1;
        int best_prio = 1e9;

        for (int i = 0; i < n; i++) {
            if (temp[i].at <= current_time && !temp[i].completed && temp[i].rem_bt > 0) {
                if (temp[i].priority < best_prio) {
                    best_prio = temp[i].priority;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            current_time++;
        } else {
            if (temp[idx].first_response_time == -1) {
                temp[idx].first_response_time = current_time;
                temp[idx].rt = current_time - temp[idx].at;
            }

            temp[idx].rem_bt--;
            current_time++;

            if (temp[idx].rem_bt == 0) {
                temp[idx].ct = current_time;
                temp[idx].tat = temp[idx].ct - temp[idx].at;
                temp[idx].wt = temp[idx].tat - temp[idx].bt;
                temp[idx].completed = true;
                completed++;
            }
        }
    }
    print_schedule_table(temp, n, "Priority Scheduling (Preemptive)");
}

/* Interactive Dispatcher for Part 6a */
void run_lab6_part_a(void) {
    printf("\n======================================================================\n");
    printf("  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C\n");
    printf("  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("======================================================================\n");

    int n;
    printf("Enter number of processes (e.g. 5): ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROC) {
        printf("Invalid input or process count too high.\n");
        return;
    }

    Process p[MAX_PROC];
    printf("\nEnter process details in sequence (PID, Arrival Time, Burst Time, Priority):\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Process P%d -> Arrival Time, Burst Time, Priority: ", i + 1);
        if (scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].priority) != 3) {
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

    if (choice == 1) schedule_fcfs(p, n);
    else if (choice == 2) schedule_sjf_non_preemptive(p, n);
    else if (choice == 3) schedule_srtf_preemptive(p, n);
    else if (choice == 4) {
        int q;
        printf("Enter Time Quantum (ms): ");
        if (scanf("%d", &q) != 1 || q <= 0) q = 2;
        schedule_round_robin(p, n, q);
    }
    else if (choice == 5) schedule_priority_non_preemptive(p, n);
    else if (choice == 6) schedule_priority_preemptive(p, n);
    else printf("Invalid algorithm selection.\n");
}


/* ============================================================================
 * QUESTION / EXPERIMENT 6 (Part b): ROUND ROBIN BENCHMARK & QUANTUM SWEEP
 * ============================================================================
 * Given Benchmark Dataset:
 *   P1: AT = 0, BT = 10
 *   P2: AT = 1, BT = 4
 *   P3: AT = 3, BT = 6
 *   P4: AT = 4, BT = 7
 *   P5: AT = 5, BT = 2
 *
 * Perform simulation for time quantum q = 1 to 10 ms, tabulate metrics,
 * and identify the optimal quantum values for TAT, WT, and RT.
 * ============================================================================ */

void run_lab6_part_b(void) {
    printf("\n======================================================================\n");
    printf("  OS LAB 6 (Part b): Round Robin Parametric Time Quantum Sweep (q=1..10)\n");
    printf("  Benchmark: P1(0,10), P2(1,4), P3(3,6), P4(4,7), P5(5,2)\n");
    printf("  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("======================================================================\n");

    Process benchmark[5] = {
        {1, 0, 10, 0, 0, 0, 0, 0, 10, -1, false},
        {2, 1,  4, 0, 0, 0, 0, 0,  4, -1, false},
        {3, 3,  6, 0, 0, 0, 0, 0,  6, -1, false},
        {4, 4,  7, 0, 0, 0, 0, 0,  7, -1, false},
        {5, 5,  2, 0, 0, 0, 0, 0,  2, -1, false}
    };
    int n = 5;

    printf("\n%-14s %-16s %-16s %-16s %-12s\n",
           "Quantum (q)", "Avg TAT (ms)", "Avg WT (ms)", "Avg RT (ms)", "Context Switches");
    printf("--------------------------------------------------------------------------------\n");

    float min_tat = 1e9, min_wt = 1e9, min_rt = 1e9;
    int opt_q_tat = 1, opt_q_wt = 1, opt_q_rt = 1;

    for (int q = 1; q <= 10; q++) {
        Process temp[5];
        for (int i = 0; i < n; i++) {
            temp[i] = benchmark[i];
            temp[i].rem_bt = temp[i].bt;
            temp[i].first_response_time = -1;
            temp[i].completed = false;
        }

        int completed = 0, current_time = 0, switches = 0;
        int queue[500];
        int front = 0, rear = 0;
        bool in_queue[5] = {false};

        for (int i = 0; i < n; i++) {
            if (temp[i].at <= current_time) {
                queue[rear++] = i;
                in_queue[i] = true;
            }
        }

        while (completed < n) {
            if (front == rear) {
                current_time++;
                for (int i = 0; i < n; i++) {
                    if (temp[i].at <= current_time && !in_queue[i] && !temp[i].completed) {
                        queue[rear++] = i;
                        in_queue[i] = true;
                    }
                }
                continue;
            }

            int idx = queue[front++];
            in_queue[idx] = false;
            switches++;

            if (temp[idx].first_response_time == -1) {
                temp[idx].first_response_time = current_time;
                temp[idx].rt = current_time - temp[idx].at;
            }

            int slice = (temp[idx].rem_bt > q) ? q : temp[idx].rem_bt;
            temp[idx].rem_bt -= slice;
            current_time += slice;

            for (int i = 0; i < n; i++) {
                if (i != idx && temp[i].at <= current_time && !in_queue[i] && !temp[i].completed) {
                    queue[rear++] = i;
                    in_queue[i] = true;
                }
            }

            if (temp[idx].rem_bt > 0) {
                queue[rear++] = idx;
                in_queue[idx] = true;
            } else {
                temp[idx].ct = current_time;
                temp[idx].tat = temp[idx].ct - temp[idx].at;
                temp[idx].wt = temp[idx].tat - temp[idx].bt;
                temp[idx].completed = true;
                completed++;
            }
        }

        float total_tat = 0, total_wt = 0, total_rt = 0;
        for (int i = 0; i < n; i++) {
            total_tat += temp[i].tat;
            total_wt += temp[i].wt;
            total_rt += temp[i].rt;
        }
        float avg_tat = total_tat / n;
        float avg_wt = total_wt / n;
        float avg_rt = total_rt / n;

        printf("q = %-9d %-16.2f %-16.2f %-16.2f %-12d\n",
               q, avg_tat, avg_wt, avg_rt, switches);

        if (avg_tat < min_tat) { min_tat = avg_tat; opt_q_tat = q; }
        if (avg_wt < min_wt)   { min_wt = avg_wt;   opt_q_wt = q; }
        if (avg_rt < min_rt)   { min_rt = avg_rt;   opt_q_rt = q; }
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("[OPTIMAL ANALYSIS RESULTS]:\n");
    printf("  Optimal Quantum for Turnaround Time (TAT) : q = %d ms (Avg TAT = %.2f ms)\n", opt_q_tat, min_tat);
    printf("  Optimal Quantum for Waiting Time (WT)    : q = %d ms (Avg WT  = %.2f ms)\n", opt_q_wt, min_wt);
    printf("  Optimal Quantum for Response Time (RT)   : q = %d ms (Avg RT  = %.2f ms)\n", opt_q_rt, min_rt);
    printf("======================================================================\n\n");
}


/* ============================================================================
 * MAIN ENTRY POINT & INTERACTIVE MENU
 * ============================================================================ */

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        if (strcmp(argv[1], "--lab6a") == 0) {
            run_lab6_part_a();
            return 0;
        } else if (strcmp(argv[1], "--lab6b") == 0) {
            run_lab6_part_b();
            return 0;
        }
    }

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

        if (opt == 1) {
            run_lab6_part_a();
        } else if (opt == 2) {
            run_lab6_part_b();
        } else if (opt == 0) {
            printf("Exiting. Thank you!\n");
            break;
        } else {
            printf("Invalid selection.\n\n");
        }
    }

    return 0;
}
