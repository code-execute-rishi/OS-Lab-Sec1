# Experiment 11: Variable Memory Partitioning with Worst Fit Placement & External Fragmentation

**Aim:**
To implement, simulate, and analyze Variable (Dynamic) Memory Partitioning in C using the Worst Fit placement algorithm. The objectives are:
- Model physical RAM initially as a single unpartitioned free block / hole of 1024 KB.
- Implement dynamic hole splitting upon process allocation, and contiguous hole coalescing upon process deallocation.
- Evaluate Case 1 (Simultaneous Arrivals at Time T): Processes {P1 (100 KB), P2 (200 KB), P3 (300 KB), P4 (300 KB), P5 (300 KB)} arrive concurrently, observing memory exhaustion.
- Evaluate Case 2 (Dynamic Timely Ordered Events T1 to T8): Interleaved arrivals and departures (T1: P1 in, T2: P2 in, T3: P3 in, T4: P2 out, T5: P4 in, T6: P5 in, T7: P4 out, T8: P6 in) creating multiple non-contiguous memory holes.
- Implement a dedicated procedure to detect and compute External Fragmentation when total free memory exceeds process requirements but cannot satisfy contiguous allocation.

**Source:** [`exp11_variable_partitioning.c`](exp11_variable_partitioning.c)

```bash
gcc exp11_variable_partitioning.c -o exp11_variable_partitioning
./exp11_variable_partitioning
```

**Input:** the data from the lab manual is built into the program, so no input is needed.

## Output

```
========================================================================================
   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 11
   VARIABLE PARTITIONING MEMORY PLACEMENT: WORST FIT SIMULATION
   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
========================================================================================

########################################################################################
  EXPERIMENT 11 - CASE 1: SIMULTANEOUS ARRIVAL AT TIME T
  Total RAM: 1024 KB | Processes: P1(100KB), P2(200KB), P3(300KB), P4(300KB), P5(300KB)
########################################################################################

  >>> Event: Initial State (Single 1024 KB Hole) <<<
  Memory Layout Map: [| FREE:1024KB (0-1024) |]
  Occupied: 0 KB | Available: 1024 KB in 1 free hole(s)

  >>> Event: Arrival of P1 (100 KB)           <<<
  Memory Layout Map: [| P1:100KB (0-100) || FREE:924KB (100-1024) |]
  Occupied: 100 KB | Available: 924 KB in 1 free hole(s)

  >>> Event: Arrival of P2 (200 KB)           <<<
  Memory Layout Map: [| P1:100KB (0-100) || P2:200KB (100-300) || FREE:724KB (300-1024) |]
  Occupied: 300 KB | Available: 724 KB in 1 free hole(s)

  >>> Event: Arrival of P3 (300 KB)           <<<
  Memory Layout Map: [| P1:100KB (0-100) || P2:200KB (100-300) || P3:300KB (300-600) || FREE:424KB (600-1024) |]
  Occupied: 600 KB | Available: 424 KB in 1 free hole(s)

  >>> Event: Arrival of P4 (300 KB)           <<<
  Memory Layout Map: [| P1:100KB (0-100) || P2:200KB (100-300) || P3:300KB (300-600) || P4:300KB (600-900) || FREE:124KB (900-1024) |]
  Occupied: 900 KB | Available: 124 KB in 1 free hole(s)

  [ALERT] Failed to allocate P5 (300 KB) under Worst Fit!
  Total Free Memory: 124 KB | Required: 300 KB | External Fragmentation: 0 KB

########################################################################################
  EXPERIMENT 11 - CASE 2: TIMELY ORDERED ARRIVALS AND DEPARTURES (T1 TO T8)
  Total RAM: 1024 KB | Demonstrating Dynamic Fragmentation and Hole Creation
########################################################################################

  >>> Event: Initial State: 1024 KB Free Hole <<<
  Memory Layout Map: [| FREE:1024KB (0-1024) |]
  Occupied: 0 KB | Available: 1024 KB in 1 free hole(s)

  >>> Event: T1: P1 arrives (100 KB)          <<<
  Memory Layout Map: [| P1:100KB (0-100) || FREE:924KB (100-1024) |]
  Occupied: 100 KB | Available: 924 KB in 1 free hole(s)

  >>> Event: T2: P2 arrives (100 KB)          <<<
  Memory Layout Map: [| P1:100KB (0-100) || P2:100KB (100-200) || FREE:824KB (200-1024) |]
  Occupied: 200 KB | Available: 824 KB in 1 free hole(s)

  >>> Event: T3: P3 arrives (200 KB)          <<<
  Memory Layout Map: [| P1:100KB (0-100) || P2:100KB (100-200) || P3:200KB (200-400) || FREE:624KB (400-1024) |]
  Occupied: 400 KB | Available: 624 KB in 1 free hole(s)

  >>> Event: T4: P2 terminates/leaves (100 KB) <<<
  Memory Layout Map: [| P1:100KB (0-100) || FREE:100KB (100-200) || P3:200KB (200-400) || FREE:624KB (400-1024) |]
  Occupied: 300 KB | Available: 724 KB in 2 free hole(s)

  >>> Event: T5: P4 arrives (200 KB)          <<<
  Memory Layout Map: [| P1:100KB (0-100) || FREE:100KB (100-200) || P3:200KB (200-400) || P4:200KB (400-600) || FREE:424KB (600-1024) |]
  Occupied: 500 KB | Available: 524 KB in 2 free hole(s)

  >>> Event: T6: P5 arrives (200 KB)          <<<
  Memory Layout Map: [| P1:100KB (0-100) || FREE:100KB (100-200) || P3:200KB (200-400) || P4:200KB (400-600) || P5:200KB (600-800) || FREE:224KB (800-1024) |]
  Occupied: 700 KB | Available: 324 KB in 2 free hole(s)

  >>> Event: T7: P4 terminates/leaves (200 KB) <<<
  Memory Layout Map: [| P1:100KB (0-100) || FREE:100KB (100-200) || P3:200KB (200-400) || FREE:200KB (400-600) || P5:200KB (600-800) || FREE:224KB (800-1024) |]
  Occupied: 500 KB | Available: 524 KB in 3 free hole(s)

  >>> Event: T8: P6 arrives (300 KB)          <<<
  [FAILED ALLOCATION] No single contiguous hole >= 300 KB!
  Total Free Memory Available = 524 KB
  Process Size Required       = 300 KB
  External Fragmentation      = 524 KB
  -> NOTE: Free space (524 KB) exceeds required (300 KB), but is non-contiguous!
```
