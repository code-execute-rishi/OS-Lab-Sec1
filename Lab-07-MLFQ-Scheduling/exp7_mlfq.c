/*
 * ==============================================================================
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 7
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * Semester       : V Semester, B.Tech CSE
 * Institution    : Maulana Azad National Institute of Technology (MANIT) Bhopal
 * ==============================================================================
 * AIM: Simulation of Multilevel Feedback Queue (MLFQ) CPU Scheduling in C
 *      using a dynamic linked list data structure.
 *
 * SPECIFICATION:
 *   - Queue 1: Round Robin (Time Quantum = 2), CPU time allocation budget = 8
 *   - Queue 2: Round Robin (Time Quantum = 4), CPU time allocation budget = 16
 *   - Queue 3: First-Come First-Served (FCFS), CPU time allocation budget = 32
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Process node structure using linked list
typedef struct ProcessNode {
    char pid[8];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int first_cpu_time;
    int queue_level; // 1 = Q1 (RR2), 2 = Q2 (RR4), 3 = Q3 (FCFS)
    struct ProcessNode *next;
} ProcessNode;

// Queue structure with head and tail pointers
typedef struct {
    ProcessNode *head;
    ProcessNode *tail;
    int count;
} Queue;

void init_queue(Queue *q) {
    q->head = NULL;
    q->tail = NULL;
    q->count = 0;
}

bool is_empty(Queue *q) {
    return q->head == NULL;
}

void enqueue(Queue *q, ProcessNode *node) {
    node->next = NULL;
    if (q->tail == NULL) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->count++;
}

ProcessNode* dequeue(Queue *q) {
    if (is_empty(q)) return NULL;
    ProcessNode *node = q->head;
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    node->next = NULL;
    q->count--;
    return node;
}

// Global process storage for reporting
#define MAX_PROC 10
ProcessNode all_processes[MAX_PROC];
int total_processes = 0;

void print_gantt_entry(int start, int end, const char *pid, int queue_no) {
    printf("  [Time %2d -> %2d] Process %s executed in Queue Q%d\n", start, end, pid, queue_no);
}

int main() {
    printf("========================================================================================\n");
    printf("   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 7\n");
    printf("   MULTILEVEL FEEDBACK QUEUE (MLFQ) SCHEDULING SIMULATION\n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1\n");
    printf("========================================================================================\n\n");

    // Benchmark workload as per Lab Manual
    const char *pids[] = {"P1", "P2", "P3", "P4"};
    int bursts[] = {4, 2, 6, 15};
    int arrivals[] = {0, 0, 0, 0};
    total_processes = 4;

    printf("Input Workload Parameters (All processes arrive in Queue 1 at t = 0):\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Process   Arrival Time   Burst Time   Initial Queue\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < total_processes; i++) {
        strcpy(all_processes[i].pid, pids[i]);
        all_processes[i].arrival_time = arrivals[i];
        all_processes[i].burst_time = bursts[i];
        all_processes[i].remaining_time = bursts[i];
        all_processes[i].completion_time = 0;
        all_processes[i].turnaround_time = 0;
        all_processes[i].waiting_time = 0;
        all_processes[i].response_time = -1;
        all_processes[i].first_cpu_time = -1;
        all_processes[i].queue_level = 1;
        all_processes[i].next = NULL;
        printf("    %-8s %-14d %-12d Q1 (RR, q=2)\n", pids[i], arrivals[i], bursts[i]);
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    // Initialize the three queues
    Queue q1, q2, q3;
    init_queue(&q1);
    init_queue(&q2);
    init_queue(&q3);

    // Enqueue all initial processes into Queue 1
    for (int i = 0; i < total_processes; i++) {
        enqueue(&q1, &all_processes[i]);
    }

    int current_time = 0;
    int completed = 0;

    printf("Gantt Chart & Multilevel Execution Trace:\n");
    printf("----------------------------------------------------------------------------------------\n");

    while (completed < total_processes) {
        // -------------------------------------------------------------
        // Queue 1: Round Robin (Time Quantum = 2), CPU Time Slice = 8
        // -------------------------------------------------------------
        if (!is_empty(&q1)) {
            int q1_time_spent = 0;
            const int q1_budget = 8;
            const int q1_tq = 2;

            while (!is_empty(&q1) && q1_time_spent < q1_budget) {
                ProcessNode *curr = dequeue(&q1);

                if (curr->first_cpu_time == -1) {
                    curr->first_cpu_time = current_time;
                    curr->response_time = current_time - curr->arrival_time;
                }

                int run_time = (curr->remaining_time > q1_tq) ? q1_tq : curr->remaining_time;
                // Check if running exceeds budget
                if (q1_time_spent + run_time > q1_budget) {
                    run_time = q1_budget - q1_time_spent;
                }

                print_gantt_entry(current_time, current_time + run_time, curr->pid, 1);
                current_time += run_time;
                curr->remaining_time -= run_time;
                q1_time_spent += run_time;

                if (curr->remaining_time == 0) {
                    curr->completion_time = current_time;
                    curr->turnaround_time = curr->completion_time - curr->arrival_time;
                    curr->waiting_time = curr->turnaround_time - curr->burst_time;
                    completed++;
                    printf("  >> Process %s COMPLETED at t = %d ms\n", curr->pid, current_time);
                } else {
                    // Demote to Queue 2 (RR, q = 4)
                    curr->queue_level = 2;
                    enqueue(&q2, curr);
                    printf("  >> Process %s demoted to Queue Q2 (Remaining BT = %d ms)\n", curr->pid, curr->remaining_time);
                }
            }
        }
        // -------------------------------------------------------------
        // Queue 2: Round Robin (Time Quantum = 4), CPU Time Slice = 16
        // -------------------------------------------------------------
        else if (!is_empty(&q2)) {
            int q2_time_spent = 0;
            const int q2_budget = 16;
            const int q2_tq = 4;

            while (!is_empty(&q2) && q2_time_spent < q2_budget) {
                // If higher queue received jobs, preemption would occur (not here since all arrived at t=0)
                ProcessNode *curr = dequeue(&q2);

                if (curr->first_cpu_time == -1) {
                    curr->first_cpu_time = current_time;
                    curr->response_time = current_time - curr->arrival_time;
                }

                int run_time = (curr->remaining_time > q2_tq) ? q2_tq : curr->remaining_time;
                if (q2_time_spent + run_time > q2_budget) {
                    run_time = q2_budget - q2_time_spent;
                }

                print_gantt_entry(current_time, current_time + run_time, curr->pid, 2);
                current_time += run_time;
                curr->remaining_time -= run_time;
                q2_time_spent += run_time;

                if (curr->remaining_time == 0) {
                    curr->completion_time = current_time;
                    curr->turnaround_time = curr->completion_time - curr->arrival_time;
                    curr->waiting_time = curr->turnaround_time - curr->burst_time;
                    completed++;
                    printf("  >> Process %s COMPLETED at t = %d ms\n", curr->pid, current_time);
                } else {
                    // Demote to Queue 3 (FCFS)
                    curr->queue_level = 3;
                    enqueue(&q3, curr);
                    printf("  >> Process %s demoted to Queue Q3 (Remaining BT = %d ms)\n", curr->pid, curr->remaining_time);
                }
            }
        }
        // -------------------------------------------------------------
        // Queue 3: First-Come First-Served (FCFS), CPU Time Slice = 32
        // -------------------------------------------------------------
        else if (!is_empty(&q3)) {
            int q3_time_spent = 0;
            const int q3_budget = 32;

            while (!is_empty(&q3) && q3_time_spent < q3_budget) {
                ProcessNode *curr = dequeue(&q3);

                if (curr->first_cpu_time == -1) {
                    curr->first_cpu_time = current_time;
                    curr->response_time = current_time - curr->arrival_time;
                }

                int run_time = curr->remaining_time;
                if (q3_time_spent + run_time > q3_budget) {
                    run_time = q3_budget - q3_time_spent;
                }

                print_gantt_entry(current_time, current_time + run_time, curr->pid, 3);
                current_time += run_time;
                curr->remaining_time -= run_time;
                q3_time_spent += run_time;

                if (curr->remaining_time == 0) {
                    curr->completion_time = current_time;
                    curr->turnaround_time = curr->completion_time - curr->arrival_time;
                    curr->waiting_time = curr->turnaround_time - curr->burst_time;
                    completed++;
                    printf("  >> Process %s COMPLETED at t = %d ms\n", curr->pid, current_time);
                } else {
                    // Re-enqueue in Queue 3 if budget expired before job finished
                    enqueue(&q3, curr);
                }
            }
        }
        else {
            current_time++;
        }
    }

    printf("----------------------------------------------------------------------------------------\n\n");

    // Print Final Performance Metrics Table
    printf("FINAL MLFQ PERFORMANCE REPORT:\n");
    printf("========================================================================================\n");
    printf("  PID   Arrival   Burst   Completion(CT)   Turnaround(TAT)   Waiting(WT)   Response(RT)\n");
    printf("----------------------------------------------------------------------------------------\n");

    double total_tat = 0, total_wt = 0, total_rt = 0;
    for (int i = 0; i < total_processes; i++) {
        printf("  %-5s %-9d %-7d %-16d %-17d %-13d %-12d\n",
               all_processes[i].pid,
               all_processes[i].arrival_time,
               all_processes[i].burst_time,
               all_processes[i].completion_time,
               all_processes[i].turnaround_time,
               all_processes[i].waiting_time,
               all_processes[i].response_time);
        total_tat += all_processes[i].turnaround_time;
        total_wt  += all_processes[i].waiting_time;
        total_rt  += all_processes[i].response_time;
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("  AVERAGES:  Average TAT = %.2f ms  |  Average WT = %.2f ms  |  Average RT = %.2f ms\n",
           total_tat / total_processes,
           total_wt / total_processes,
           total_rt / total_processes);
    printf("========================================================================================\n");

    return 0;
}
