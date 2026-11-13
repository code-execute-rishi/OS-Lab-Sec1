# Experiment 6: CPU Scheduling Algorithms & Round Robin Time Quantum Optimization

**Aim:**
1. Part A (Generic CPU Scheduler in C): Write a modular, menu-driven C program implementing six CPU scheduling algorithms: First-Come First-Served (FCFS), Shortest Job First (SJF - Non-Preemptive), Shortest Remaining Time First (SRTF - Preemptive SJF), Round Robin (RR), Non-Preemptive Priority, and Preemptive Priority. The program dynamically inputs process records (PID, Arrival Time, Burst Time, Priority), executes the chosen algorithm (prompting for Time Quantum if RR is selected), and computes Completion Time (CT), Turnaround Time (TAT), Waiting Time (WT), and Response Time (RT) per process and their averages.
2. Part B (Parametric RR Simulation & Optimal Quantum Analysis): Write a simulation program in C to execute Round Robin scheduling on the specified benchmark dataset across time quantum values from 1 ms to 10 ms, analyze the resulting TAT, WT, and RT trajectories, and determine the optimal time quantum.

| Process | Arrival Time (ms) | Burst Time (ms) | Priority |
|---------|-------------------|-----------------|----------|
| P1 | 0 | 10 | 3 |
| P2 | 1 | 4  | 1 |
| P3 | 3 | 6  | 4 |
| P4 | 4 | 7  | 2 |
| P5 | 5 | 2  | 5 |

Lower priority number = higher priority.

**Source:** [`cpu_scheduler.c`](cpu_scheduler.c)

```bash
gcc cpu_scheduler.c -o cpu_scheduler
./cpu_scheduler            # menu: 1 = Part A (interactive), 2 = Part B (quantum sweep)
./cpu_scheduler --lab6a    # Part A directly
./cpu_scheduler --lab6b    # Part B directly
```

## Part A: Generic CPU scheduler

**Input format:** number of processes, then one line per process with `arrival burst priority`, then the algorithm number (1–6). For Round Robin (4), the time quantum comes last.

**Input** ([`input_rr.txt`](input_rr.txt)). The other input files differ only in the algorithm number.

```
5
0 10 3
1 4 1
3 6 4
4 7 2
5 2 5
4
2
```

### 1. FCFS

Run `./cpu_scheduler --lab6a` and type the values, or run `./cpu_scheduler --lab6a < input_fcfs.txt`.

```
$ ./cpu_scheduler --lab6a

======================================================================
  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C
  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
======================================================================
Enter number of processes (e.g. 5): 5

Enter process details in sequence (PID, Arrival Time, Burst Time, Priority):
Process P1 -> Arrival Time, Burst Time, Priority: 0 10 3
Process P2 -> Arrival Time, Burst Time, Priority: 1 4 1
Process P3 -> Arrival Time, Burst Time, Priority: 3 6 4
Process P4 -> Arrival Time, Burst Time, Priority: 4 7 2
Process P5 -> Arrival Time, Burst Time, Priority: 5 2 5

Select CPU Scheduling Algorithm:
  1. FCFS (First Come First Serve)
  2. SJF (Shortest Job First - Non-Preemptive)
  3. SRTF (Shortest Remaining Time First - Preemptive SJF)
  4. Round Robin (RR)
  5. Priority (Non-Preemptive)
  6. Priority (Preemptive)
Enter selection [1-6]: 1

========================================================================================
  CPU SCHEDULING ALGORITHM: First Come First Serve (FCFS)
========================================================================================
PID      Arrival(AT)  Burst(BT)    Priority   CT       TAT      WT       RT
----------------------------------------------------------------------------------------
P1       0            10           3          10       10       0        0
P2       1            4            1          14       13       9        9
P3       3            6            4          20       17       11       11
P4       4            7            2          27       23       16       16
P5       5            2            5          29       24       22       22
----------------------------------------------------------------------------------------
Average Turnaround Time (TAT) : 17.40 ms
Average Waiting Time (WT)    : 11.60 ms
Average Response Time (RT)   : 11.60 ms
========================================================================================
```

### 2. SJF (Non-Preemptive)

Run `./cpu_scheduler --lab6a` and type the values, or run `./cpu_scheduler --lab6a < input_sjf.txt`.

```
$ ./cpu_scheduler --lab6a

======================================================================
  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C
  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
======================================================================
Enter number of processes (e.g. 5): 5

Enter process details in sequence (PID, Arrival Time, Burst Time, Priority):
Process P1 -> Arrival Time, Burst Time, Priority: 0 10 3
Process P2 -> Arrival Time, Burst Time, Priority: 1 4 1
Process P3 -> Arrival Time, Burst Time, Priority: 3 6 4
Process P4 -> Arrival Time, Burst Time, Priority: 4 7 2
Process P5 -> Arrival Time, Burst Time, Priority: 5 2 5

Select CPU Scheduling Algorithm:
  1. FCFS (First Come First Serve)
  2. SJF (Shortest Job First - Non-Preemptive)
  3. SRTF (Shortest Remaining Time First - Preemptive SJF)
  4. Round Robin (RR)
  5. Priority (Non-Preemptive)
  6. Priority (Preemptive)
Enter selection [1-6]: 2

========================================================================================
  CPU SCHEDULING ALGORITHM: Shortest Job First (SJF - Non-Preemptive)
========================================================================================
PID      Arrival(AT)  Burst(BT)    Priority   CT       TAT      WT       RT
----------------------------------------------------------------------------------------
P1       0            10           3          10       10       0        0
P2       1            4            1          16       15       11       11
P3       3            6            4          22       19       13       13
P4       4            7            2          29       25       18       18
P5       5            2            5          12       7        5        5
----------------------------------------------------------------------------------------
Average Turnaround Time (TAT) : 15.20 ms
Average Waiting Time (WT)    : 9.40 ms
Average Response Time (RT)   : 9.40 ms
========================================================================================
```

### 3. SRTF (Preemptive SJF)

Run `./cpu_scheduler --lab6a` and type the values, or run `./cpu_scheduler --lab6a < input_srtf.txt`.

```
$ ./cpu_scheduler --lab6a

======================================================================
  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C
  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
======================================================================
Enter number of processes (e.g. 5): 5

Enter process details in sequence (PID, Arrival Time, Burst Time, Priority):
Process P1 -> Arrival Time, Burst Time, Priority: 0 10 3
Process P2 -> Arrival Time, Burst Time, Priority: 1 4 1
Process P3 -> Arrival Time, Burst Time, Priority: 3 6 4
Process P4 -> Arrival Time, Burst Time, Priority: 4 7 2
Process P5 -> Arrival Time, Burst Time, Priority: 5 2 5

Select CPU Scheduling Algorithm:
  1. FCFS (First Come First Serve)
  2. SJF (Shortest Job First - Non-Preemptive)
  3. SRTF (Shortest Remaining Time First - Preemptive SJF)
  4. Round Robin (RR)
  5. Priority (Non-Preemptive)
  6. Priority (Preemptive)
Enter selection [1-6]: 3

========================================================================================
  CPU SCHEDULING ALGORITHM: Shortest Remaining Time First (SRTF - Preemptive)
========================================================================================
PID      Arrival(AT)  Burst(BT)    Priority   CT       TAT      WT       RT
----------------------------------------------------------------------------------------
P1       0            10           3          29       29       19       0
P2       1            4            1          5        4        0        0
P3       3            6            4          13       10       4        4
P4       4            7            2          20       16       9        9
P5       5            2            5          7        2        0        0
----------------------------------------------------------------------------------------
Average Turnaround Time (TAT) : 12.20 ms
Average Waiting Time (WT)    : 6.40 ms
Average Response Time (RT)   : 2.60 ms
========================================================================================
```

### 4. Round Robin (Time Quantum = 2 ms)

Run `./cpu_scheduler --lab6a` and type the values, or run `./cpu_scheduler --lab6a < input_rr.txt`.

```
$ ./cpu_scheduler --lab6a

======================================================================
  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C
  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
======================================================================
Enter number of processes (e.g. 5): 5

Enter process details in sequence (PID, Arrival Time, Burst Time, Priority):
Process P1 -> Arrival Time, Burst Time, Priority: 0 10 3
Process P2 -> Arrival Time, Burst Time, Priority: 1 4 1
Process P3 -> Arrival Time, Burst Time, Priority: 3 6 4
Process P4 -> Arrival Time, Burst Time, Priority: 4 7 2
Process P5 -> Arrival Time, Burst Time, Priority: 5 2 5

Select CPU Scheduling Algorithm:
  1. FCFS (First Come First Serve)
  2. SJF (Shortest Job First - Non-Preemptive)
  3. SRTF (Shortest Remaining Time First - Preemptive SJF)
  4. Round Robin (RR)
  5. Priority (Non-Preemptive)
  6. Priority (Preemptive)
Enter selection [1-6]: 4
Enter Time Quantum (ms): 2

========================================================================================
  CPU SCHEDULING ALGORITHM: Round Robin (RR, Quantum = 2 ms)
========================================================================================
PID      Arrival(AT)  Burst(BT)    Priority   CT       TAT      WT       RT
----------------------------------------------------------------------------------------
P1       0            10           3          28       28       18       0
P2       1            4            1          12       11       7        1
P3       3            6            4          24       21       15       3
P4       4            7            2          29       25       18       4
P5       5            2            5          14       9        7        7
----------------------------------------------------------------------------------------
Average Turnaround Time (TAT) : 18.80 ms
Average Waiting Time (WT)    : 13.00 ms
Average Response Time (RT)   : 3.00 ms
========================================================================================
```

### 5. Priority (Non-Preemptive)

Run `./cpu_scheduler --lab6a` and type the values, or run `./cpu_scheduler --lab6a < input_priority_np.txt`.

```
$ ./cpu_scheduler --lab6a

======================================================================
  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C
  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
======================================================================
Enter number of processes (e.g. 5): 5

Enter process details in sequence (PID, Arrival Time, Burst Time, Priority):
Process P1 -> Arrival Time, Burst Time, Priority: 0 10 3
Process P2 -> Arrival Time, Burst Time, Priority: 1 4 1
Process P3 -> Arrival Time, Burst Time, Priority: 3 6 4
Process P4 -> Arrival Time, Burst Time, Priority: 4 7 2
Process P5 -> Arrival Time, Burst Time, Priority: 5 2 5

Select CPU Scheduling Algorithm:
  1. FCFS (First Come First Serve)
  2. SJF (Shortest Job First - Non-Preemptive)
  3. SRTF (Shortest Remaining Time First - Preemptive SJF)
  4. Round Robin (RR)
  5. Priority (Non-Preemptive)
  6. Priority (Preemptive)
Enter selection [1-6]: 5

========================================================================================
  CPU SCHEDULING ALGORITHM: Priority Scheduling (Non-Preemptive)
========================================================================================
PID      Arrival(AT)  Burst(BT)    Priority   CT       TAT      WT       RT
----------------------------------------------------------------------------------------
P1       0            10           3          10       10       0        0
P2       1            4            1          14       13       9        9
P3       3            6            4          27       24       18       18
P4       4            7            2          21       17       10       10
P5       5            2            5          29       24       22       22
----------------------------------------------------------------------------------------
Average Turnaround Time (TAT) : 17.60 ms
Average Waiting Time (WT)    : 11.80 ms
Average Response Time (RT)   : 11.80 ms
========================================================================================
```

### 6. Priority (Preemptive)

Run `./cpu_scheduler --lab6a` and type the values, or run `./cpu_scheduler --lab6a < input_priority_p.txt`.

```
$ ./cpu_scheduler --lab6a

======================================================================
  OS LAB 6 (Part a): Generic CPU Scheduling Algorithms in C
  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
======================================================================
Enter number of processes (e.g. 5): 5

Enter process details in sequence (PID, Arrival Time, Burst Time, Priority):
Process P1 -> Arrival Time, Burst Time, Priority: 0 10 3
Process P2 -> Arrival Time, Burst Time, Priority: 1 4 1
Process P3 -> Arrival Time, Burst Time, Priority: 3 6 4
Process P4 -> Arrival Time, Burst Time, Priority: 4 7 2
Process P5 -> Arrival Time, Burst Time, Priority: 5 2 5

Select CPU Scheduling Algorithm:
  1. FCFS (First Come First Serve)
  2. SJF (Shortest Job First - Non-Preemptive)
  3. SRTF (Shortest Remaining Time First - Preemptive SJF)
  4. Round Robin (RR)
  5. Priority (Non-Preemptive)
  6. Priority (Preemptive)
Enter selection [1-6]: 6

========================================================================================
  CPU SCHEDULING ALGORITHM: Priority Scheduling (Preemptive)
========================================================================================
PID      Arrival(AT)  Burst(BT)    Priority   CT       TAT      WT       RT
----------------------------------------------------------------------------------------
P1       0            10           3          21       21       11       0
P2       1            4            1          5        4        0        0
P3       3            6            4          27       24       18       18
P4       4            7            2          12       8        1        1
P5       5            2            5          29       24       22       22
----------------------------------------------------------------------------------------
Average Turnaround Time (TAT) : 16.20 ms
Average Waiting Time (WT)    : 10.40 ms
Average Response Time (RT)   : 8.20 ms
========================================================================================
```


## Part B: Round Robin time quantum sweep (q = 1 to 10 ms)

```bash
./cpu_scheduler --lab6b
```

```
======================================================================
  OS LAB 6 (Part b): Round Robin Parametric Time Quantum Sweep (q=1..10)
  Benchmark: P1(0,10), P2(1,4), P3(3,6), P4(4,7), P5(5,2)
  Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
======================================================================

Quantum (q)    Avg TAT (ms)     Avg WT (ms)      Avg RT (ms)      Context Switches
--------------------------------------------------------------------------------
q = 1         19.00            13.20            1.20             29
q = 2         18.80            13.00            3.00             15
q = 3         20.00            14.20            4.60             12
q = 4         19.40            13.60            6.20             9
q = 5         19.80            14.00            6.80             8
q = 6         18.80            13.00            8.20             7
q = 7         18.80            13.00            9.20             6
q = 8         19.60            13.80            10.00            6
q = 9         20.40            14.60            10.80            6
q = 10        17.40            11.60            11.60            5
--------------------------------------------------------------------------------
[OPTIMAL ANALYSIS RESULTS]:
  Optimal Quantum for Turnaround Time (TAT) : q = 10 ms (Avg TAT = 17.40 ms)
  Optimal Quantum for Waiting Time (WT)    : q = 10 ms (Avg WT  = 11.60 ms)
  Optimal Quantum for Response Time (RT)   : q = 1 ms (Avg RT  = 1.20 ms)
======================================================================
```
