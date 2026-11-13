# Experiment 10: Fixed Memory Partitioning Placement Schemes (First Fit & Best Fit)

**Aim:**
To implement, execute, and evaluate Fixed Partitioning Memory Placement Schemes in C under both Equal-Size and Unequal-Size memory block partitioning architectures. The assignment requires:
- Implementing two distinct placement strategies: First Fit and Best Fit.
- Evaluating Case 1 (Equal Block Sizes): Total Main Memory = 1024 KB divided into 4 fixed partitions of 256 KB each, accommodating processes of sizes {100 KB, 150 KB, 200 KB, 250 KB}.
- Evaluating Case 2 (Unequal Block Sizes): Total Main Memory = 1024 KB divided into 4 fixed partitions of sizes {128 KB, 128 KB, 256 KB, 512 KB}, accommodating processes of sizes {100 KB, 150 KB, 200 KB, 250 KB}.
- Implementing separate computational procedures for Internal Fragmentation per block, Total Internal Fragmentation, and External Fragmentation.

**Source:** [`exp10_fixed_partitioning.c`](exp10_fixed_partitioning.c)

```bash
gcc exp10_fixed_partitioning.c -o exp10_fixed_partitioning
./exp10_fixed_partitioning
```

**Input:** the data from the lab manual is built into the program, so no input is needed.

## Output

```
========================================================================================
   OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 10
   FIXED PARTITIONING MEMORY PLACEMENT: FIRST FIT & BEST FIT
   Student: Aradhya Jain | Scholar No: 24112011111 | Section: CSE-1
========================================================================================

########################################################################################
  EXPERIMENT 10 - CASE 1: EQUAL BLOCK SIZES (Total RAM = 1024 KB, 4x256 KB)
  Processes: P1 = 100 KB, P2 = 150 KB, P3 = 200 KB, P4 = 250 KB
########################################################################################

>>> FIRST FIT ALGORITHM - CASE 1 (EQUAL SIZES) <<<
----------------------------------------------------------------------------------------
  Block ID   Capacity(KB)   Occupant   Process Size(KB)   Internal Frag(KB)   Status
----------------------------------------------------------------------------------------
  Block 1    256            P1        100                156                 ALLOCATED
  Block 2    256            P2        150                106                 ALLOCATED
  Block 3    256            P3        200                56                  ALLOCATED
  Block 4    256            P4        250                6                   ALLOCATED
----------------------------------------------------------------------------------------
  Total Internal Fragmentation: 324 KB
  External Fragmentation       : 0 KB

>>> BEST FIT ALGORITHM - CASE 1 (EQUAL SIZES) <<<
----------------------------------------------------------------------------------------
  Block ID   Capacity(KB)   Occupant   Process Size(KB)   Internal Frag(KB)   Status
----------------------------------------------------------------------------------------
  Block 1    256            P1        100                156                 ALLOCATED
  Block 2    256            P2        150                106                 ALLOCATED
  Block 3    256            P3        200                56                  ALLOCATED
  Block 4    256            P4        250                6                   ALLOCATED
----------------------------------------------------------------------------------------
  Total Internal Fragmentation: 324 KB
  External Fragmentation       : 0 KB

########################################################################################
  EXPERIMENT 10 - CASE 2: UNEQUAL BLOCK SIZES (Total RAM = 1024 KB: 128, 128, 256, 512)
  Processes: P1 = 100 KB, P2 = 150 KB, P3 = 200 KB, P4 = 250 KB
########################################################################################

>>> FIRST FIT ALGORITHM - CASE 2 (UNEQUAL SIZES) <<<
----------------------------------------------------------------------------------------
  Block ID   Capacity(KB)   Occupant   Process Size(KB)   Internal Frag(KB)   Status
----------------------------------------------------------------------------------------
  Block 1    128            P1        100                28                  ALLOCATED
  Block 2    128            ---        ---                0 (Free Block)      FREE
  Block 3    256            P2        150                106                 ALLOCATED
  Block 4    512            P3        200                312                 ALLOCATED
----------------------------------------------------------------------------------------
  Total Internal Fragmentation: 446 KB
  External Fragmentation       : 0 KB
  [ALERT] Process P4 (250 KB) could NOT be accommodated!

>>> BEST FIT ALGORITHM - CASE 2 (UNEQUAL SIZES) <<<
----------------------------------------------------------------------------------------
  Block ID   Capacity(KB)   Occupant   Process Size(KB)   Internal Frag(KB)   Status
----------------------------------------------------------------------------------------
  Block 1    128            P1        100                28                  ALLOCATED
  Block 2    128            ---        ---                0 (Free Block)      FREE
  Block 3    256            P2        150                106                 ALLOCATED
  Block 4    512            P3        200                312                 ALLOCATED
----------------------------------------------------------------------------------------
  Total Internal Fragmentation: 446 KB
  External Fragmentation       : 0 KB
  [ALERT] Process P4 (250 KB) could NOT be accommodated!
```
