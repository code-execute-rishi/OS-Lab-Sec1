# Experiment 12: Banker's Deadlock Avoidance & Producer-Consumer Bounded Buffer

**Aim:**
To implement, simulate, and analyze two canonical operating systems synchronization and resource coordination problems in C:
- Part A (Banker's Algorithm for Deadlock Avoidance): Implement Edsger Dijkstra's Banker's Algorithm for a multi-resource system (M processes, N resource types). Implement dedicated separate procedures for the Safety Algorithm and the Resource-Request Allocation Algorithm utilizing all requisite data structures: TOTAL[1×N], MAX[M×N], ALLOCATION[M×N], NEED[M×N], and AVAILABLE[1×N]. Determine whether requested states yield a guaranteed Safe State (producing a valid execution sequence) or an Unsafe State triggering rollback.
- Part B (The Classical Producer-Consumer Problem): Model the bounded-buffer synchronization problem using a shared circular queue. Guarantee mutual exclusion and boundary invariant protection (Producer cannot add to a full buffer; Consumer cannot extract from an empty buffer). Provide an interactive menu-driven interface replicating the experimental execution log specified in the laboratory manual.

**Source:** [`exp12_banker_prodcons.c`](exp12_banker_prodcons.c)

```bash
gcc exp12_banker_prodcons.c -o exp12_banker_prodcons
./exp12_banker_prodcons
```

**Input:** the data from the lab manual is built into the program, so no input is needed.

The Producer-Consumer part runs the menu choices from the lab manual's execution log automatically; it does not read from the keyboard.

## Output

```
========================================================================================
  PART A: BANKER'S ALGORITHM DEADLOCK AVOIDANCE SIMULATION
  Processes M = 5 (P0 to P4) | Resource Types N = 3 (A, B, C)
========================================================================================

System Resource Configuration Matrices:
----------------------------------------------------------------------------------------
  PID    Allocation (A B C)    Maximum Claim (A B C)    Need Matrix (A B C)
----------------------------------------------------------------------------------------
  P0     0  1  0                7  5  3                    7  4  3
  P1     2  0  0                3  2  2                    1  2  2
  P2     3  0  2                9  0  2                    6  0  0
  P3     2  1  1                2  2  2                    0  1  1
  P4     0  0  2                4  3  3                    4  3  1
----------------------------------------------------------------------------------------
  Total System Resources: [A=10, B=5, C=7]
  Currently Available   : [A=3, B=3, C=2]

Executing Safety Algorithm:
  >>> STATUS: SAFE STATE <<<
  Valid Safe Sequence Found: < P1 -> P3 -> P4 -> P0 -> P2 >

Resource-Request Simulation:
----------------------------------------------------------------------------------------
Case 1: Process P1 requests (1, 0, 2):
  [SUCCESS] Request granted safely! System remains in a SAFE STATE.
  Safe Execution Sequence: < P1 P3 P4 P0 P2 >

Case 2: Process P4 requests (3, 3, 0):
  [WAIT] Resources not available. Process P4 must wait.

Case 3: Process P0 requests (0, 2, 0):
  [UNSAFE] Granting request leads to an UNSAFE STATE! Request rolled back.
========================================================================================

========================================================================================
  PART B: PRODUCER-CONSUMER BOUNDED BUFFER SIMULATION
  Buffer Capacity = 3 Items
========================================================================================

1. Producer
2. Consumer
3. Exit
Enter your choice: 2
Buffer is empty!!
1. Producer
2. Consumer
3. Exit
Enter your choice: 1
Producer produces item 1
1. Producer
2. Consumer
3. Exit
Enter your choice: 1
Producer produces item 2
1. Producer
2. Consumer
3. Exit
Enter your choice: 1
Producer produces item 3
1. Producer
2. Consumer
3. Exit
Enter your choice: 2
Consumer consumes item 1
1. Producer
2. Consumer
3. Exit
Enter your choice: 1
Producer produces item 4
1. Producer
2. Consumer
3. Exit
Enter your choice: 2
Consumer consumes item 2
1. Producer
2. Consumer
3. Exit
Enter your choice: 2
Consumer consumes item 3
1. Producer
2. Consumer
3. Exit
Enter your choice: 2
Consumer consumes item 4
1. Producer
2. Consumer
3. Exit
Enter your choice: 3
Exiting Producer-Consumer simulation.
========================================================================================
```
