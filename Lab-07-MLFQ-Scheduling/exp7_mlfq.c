/*
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 7
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * AIM: Simulation of Multilevel Feedback Queue (MLFQ) CPU Scheduling
 *   - Q1: Round Robin (q = 2),  budget = 8
 *   - Q2: Round Robin (q = 4),  budget = 16
 *   - Q3: FCFS,                 budget = 32
 */

#include <stdio.h>

#define N 4

char *pid[N] = {"P1", "P2", "P3", "P4"};
int at[N] = {0, 0, 0, 0};    // arrival time
int bt[N] = {4, 2, 6, 15};   // burst time
int rem[N], ct[N], rt[N];    // remaining, completion, response time

// 3 queues (index 0,1,2 = Q1,Q2,Q3) stored as plain arrays
int queue[3][100], front[3], rear[3];
int t = 0, done = 0;

// Run one queue level until it is empty or its budget is used up
void run_queue(int lv, int tq, int budget) {
    int used = 0;
    while (front[lv] < rear[lv] && used < budget) {
        int i = queue[lv][front[lv]++];          // dequeue
        if (rt[i] == -1) rt[i] = t - at[i];      // first time on CPU

        int run = rem[i] < tq ? rem[i] : tq;     // run for one quantum
        if (used + run > budget) run = budget - used;

        printf("  [Time %2d -> %2d] Process %s executed in Queue Q%d\n", t, t + run, pid[i], lv + 1);
        t += run;
        rem[i] -= run;
        used += run;

        if (rem[i] == 0) {
            ct[i] = t;
            done++;
            printf("  >> Process %s COMPLETED at t = %d ms\n", pid[i], t);
        } else if (lv < 2) {
            queue[lv + 1][rear[lv + 1]++] = i;   // not finished: demote to next queue
            printf("  >> Process %s demoted to Queue Q%d (Remaining BT = %d ms)\n", pid[i], lv + 2, rem[i]);
        } else {
            queue[lv][rear[lv]++] = i;           // Q3: back of the same queue
        }
    }
}

int main() {
    printf("========================================================================================\n");
    printf("   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 7\n");
    printf("   MULTILEVEL FEEDBACK QUEUE (MLFQ) SCHEDULING SIMULATION\n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("========================================================================================\n\n");

    printf("Input Workload Parameters (All processes arrive in Queue 1 at t = 0):\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Process   Arrival Time   Burst Time   Initial Queue\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < N; i++) {
        rem[i] = bt[i];
        rt[i] = -1;
        queue[0][rear[0]++] = i;                 // everyone starts in Q1
        printf("    %-8s %-14d %-12d Q1 (RR, q=2)\n", pid[i], at[i], bt[i]);
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("Gantt Chart & Multilevel Execution Trace:\n");
    printf("----------------------------------------------------------------------------------------\n");
    // Always serve the highest non-empty queue
    while (done < N) {
        if (front[0] < rear[0])      run_queue(0, 2, 8);
        else if (front[1] < rear[1]) run_queue(1, 4, 16);
        else if (front[2] < rear[2]) run_queue(2, 32, 32);   // FCFS = no real quantum
        else t++;
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("FINAL MLFQ PERFORMANCE REPORT:\n");
    printf("========================================================================================\n");
    printf("  PID   Arrival   Burst   Completion(CT)   Turnaround(TAT)   Waiting(WT)   Response(RT)\n");
    printf("----------------------------------------------------------------------------------------\n");
    double sum_tat = 0, sum_wt = 0, sum_rt = 0;
    for (int i = 0; i < N; i++) {
        int tat = ct[i] - at[i];                 // TAT = CT - AT
        int wt = tat - bt[i];                    // WT  = TAT - BT
        printf("  %-5s %-9d %-7d %-16d %-17d %-13d %-12d\n", pid[i], at[i], bt[i], ct[i], tat, wt, rt[i]);
        sum_tat += tat;
        sum_wt += wt;
        sum_rt += rt[i];
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("  AVERAGES:  Average TAT = %.2f ms  |  Average WT = %.2f ms  |  Average RT = %.2f ms\n",
           sum_tat / N, sum_wt / N, sum_rt / N);
    printf("========================================================================================\n");
    return 0;
}
