# Experiment 7: Multilevel Feedback Queue (MLFQ) CPU Scheduling Simulation

**Aim:**
To design, implement, and simulate a Multilevel Feedback Queue (MLFQ) CPU Scheduling Algorithm in C using dynamic linked list data structures. The system architecture consists of three distinct queues with differential scheduling disciplines and CPU time quantum allocations:
- Queue 1 (Q1 - High Priority): Round Robin with Time Quantum q = 2 ms. Allocated a maximum CPU budget slice of 8 time units per cycle.
- Queue 2 (Q2 - Medium Priority): Round Robin with Time Quantum q = 4 ms. Allocated a maximum CPU budget slice of 16 time units per cycle.
- Queue 3 (Q3 - Low Priority): First-Come First-Served (FCFS). Allocated a maximum CPU budget slice of 32 time units per cycle.

The simulation evaluates the benchmark process workload specified in the laboratory manual where all processes initialize in Queue 1 at t = 0: P1 (BT = 4 ms), P2 (BT = 2 ms), P3 (BT = 6 ms), and P4 (BT = 15 ms). The program computes individual Completion Time (CT), Turnaround Time (TAT), Waiting Time (WT), and Response Time (RT), alongside whole-system performance averages.

**Source:** [`exp7_mlfq.c`](exp7_mlfq.c)

```bash
gcc exp7_mlfq.c -o exp7_mlfq
./exp7_mlfq
```

**Input:** the data from the lab manual is built into the program, so no input is needed.

## Output

```
========================================================================================
   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 7
   MULTILEVEL FEEDBACK QUEUE (MLFQ) SCHEDULING SIMULATION
   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
========================================================================================

Input Workload Parameters (All processes arrive in Queue 1 at t = 0):
----------------------------------------------------------------------------------------
  Process   Arrival Time   Burst Time   Initial Queue
----------------------------------------------------------------------------------------
    P1       0              4            Q1 (RR, q=2)
    P2       0              2            Q1 (RR, q=2)
    P3       0              6            Q1 (RR, q=2)
    P4       0              15           Q1 (RR, q=2)
----------------------------------------------------------------------------------------

Gantt Chart & Multilevel Execution Trace:
----------------------------------------------------------------------------------------
  [Time  0 ->  2] Process P1 executed in Queue Q1
  >> Process P1 demoted to Queue Q2 (Remaining BT = 2 ms)
  [Time  2 ->  4] Process P2 executed in Queue Q1
  >> Process P2 COMPLETED at t = 4 ms
  [Time  4 ->  6] Process P3 executed in Queue Q1
  >> Process P3 demoted to Queue Q2 (Remaining BT = 4 ms)
  [Time  6 ->  8] Process P4 executed in Queue Q1
  >> Process P4 demoted to Queue Q2 (Remaining BT = 13 ms)
  [Time  8 -> 10] Process P1 executed in Queue Q2
  >> Process P1 COMPLETED at t = 10 ms
  [Time 10 -> 14] Process P3 executed in Queue Q2
  >> Process P3 COMPLETED at t = 14 ms
  [Time 14 -> 18] Process P4 executed in Queue Q2
  >> Process P4 demoted to Queue Q3 (Remaining BT = 9 ms)
  [Time 18 -> 27] Process P4 executed in Queue Q3
  >> Process P4 COMPLETED at t = 27 ms
----------------------------------------------------------------------------------------

FINAL MLFQ PERFORMANCE REPORT:
========================================================================================
  PID   Arrival   Burst   Completion(CT)   Turnaround(TAT)   Waiting(WT)   Response(RT)
----------------------------------------------------------------------------------------
  P1    0         4       10               10                6             0
  P2    0         2       4                4                 2             2
  P3    0         6       14               14                8             4
  P4    0         15      27               27                12            6
----------------------------------------------------------------------------------------
  AVERAGES:  Average TAT = 13.75 ms  |  Average WT = 7.00 ms  |  Average RT = 3.00 ms
========================================================================================
```
