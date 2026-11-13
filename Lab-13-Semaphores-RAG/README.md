# Experiment 13: Semaphore Implementation & Resource Allocation Graph (RAG) Deadlock Detection

**Aim:**
To implement, execute, and analyze concurrent process synchronization and deadlock detection algorithms in C:
- Part A (Implementation of Semaphores): Demonstrate how two concurrent processes/threads share a critical global variable using semaphores. Implement and contrast two foundational semaphore variants:
  - Binary Semaphore: Initialized to 1 to achieve strict Mutual Exclusion (Mutex) across critical sections, preventing race conditions.
  - Counting Semaphore: Initialized to N > 1 (e.g. 2 instances) to synchronize multiple concurrent processes accessing a finite resource pool.
- Part B (Resource Allocation Graph & Deadlock Detection): Model a single-instance multi-resource system (P1 … Pm processes, R1 … Rn resources) using directed graph data structures. Implement directed Assignment Edges (Rj → Pi) and Request Edges (Pi → Rj). Design and execute a separate cycle-detection algorithm (Depth-First Search with 3-color vertex marking) to definitively declare whether the system is Deadlock Free or in a Deadlock Detected state.

**Source:** [`exp13_semaphore_rag.c`](exp13_semaphore_rag.c)

```bash
gcc -pthread exp13_semaphore_rag.c -o exp13_semaphore_rag
./exp13_semaphore_rag
```

**Input:** the data from the lab manual is built into the program, so no input is needed.

The "Result WITHOUT Semaphore" number comes from a race condition, so it is different on every run. The thread order in the counting-semaphore part can also change.

## Output

```
========================================================================================
  PART A: SEMAPHORE IMPLEMENTATION (BINARY & COUNTING SEMAPHORES)
========================================================================================

1. Binary Semaphore Demonstration (Mutual Exclusion on Shared Variable):
----------------------------------------------------------------------------------------
  Expected Final Value (2 threads x 100000)  : 200000
  Result WITHOUT Semaphore (Race Hazard)        : 100046 (DATA INCONSISTENCY)
  Result WITH Binary Semaphore (Mutual Exclusion) : 200000 (SYNCHRONIZED - EXACT)

2. Counting Semaphore Demonstration (Resource Pool of 2 Instances shared by 4 Tasks):
----------------------------------------------------------------------------------------
  [Process P1] Requesting resource from pool (waiting on counting semaphore)...
  >>> [Process P1] ACQUIRED RESOURCE! Accessing critical shared resource unit...
  [Process P2] Requesting resource from pool (waiting on counting semaphore)...
  >>> [Process P2] ACQUIRED RESOURCE! Accessing critical shared resource unit...
  [Process P3] Requesting resource from pool (waiting on counting semaphore)...
  [Process P4] Requesting resource from pool (waiting on counting semaphore)...
  <<< [Process P1] FINISHED! Releasing resource back to pool.
  <<< [Process P2] FINISHED! Releasing resource back to pool.
  >>> [Process P3] ACQUIRED RESOURCE! Accessing critical shared resource unit...
  >>> [Process P4] ACQUIRED RESOURCE! Accessing critical shared resource unit...
  <<< [Process P4] FINISHED! Releasing resource back to pool.
  <<< [Process P3] FINISHED! Releasing resource back to pool.
========================================================================================

========================================================================================
  PART B: RESOURCE ALLOCATION GRAPH (RAG) & CYCLE DETECTION
========================================================================================

>>> SCENARIO: CASE 1 - ACYCLIC GRAPH (DEADLOCK FREE) <<<
----------------------------------------------------------------------------------------
  Processes: 3 | Single-Instance Resources: 2
  Graph Directed Edges:
    Request Edge    : P1 ----> R1 (Process requests Resource)
    Request Edge    : P2 ----> R2 (Process requests Resource)
    Assignment Edge : R1 ----> P2 (Resource allocated to Process)
    Assignment Edge : R2 ----> P3 (Resource allocated to Process)

  >>> STATUS: DEADLOCK FREE <<<
  No directed cycles detected. System is safe and deadlock-free.

>>> SCENARIO: CASE 2 - CIRCULAR DEPENDENCY (DEADLOCK CYCLE) <<<
----------------------------------------------------------------------------------------
  Processes: 2 | Single-Instance Resources: 2
  Graph Directed Edges:
    Request Edge    : P1 ----> R1 (Process requests Resource)
    Request Edge    : P2 ----> R2 (Process requests Resource)
    Assignment Edge : R1 ----> P2 (Resource allocated to Process)
    Assignment Edge : R2 ----> P1 (Resource allocated to Process)

  >>> STATUS: DEADLOCK DETECTED! <<<
  Circular Wait Condition satisfied in single-instance Resource Allocation Graph.
  Directed Cycle Identified: P1 -> R1 -> P2 -> R2 -> P1
  Processes deadlocked: P1 P2
```
