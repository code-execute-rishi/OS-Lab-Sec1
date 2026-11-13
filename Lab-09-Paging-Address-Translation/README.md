# Experiment 9: Logical to Physical Address Translation & Dynamic Paging Scheme

**Aim:**
To implement, simulate, and analyze the architectural foundations of Virtual Memory Paging in C:
- Part A (Address Translation Simulation): Develop a program translating a 15-bit Logical Address Space (32 KB, 2^15 bytes) to a 12-bit Physical Address Space (4 KB, 2^12 bytes) using a configured page size of 128 bytes (2^7 bytes). Implement bitwise manipulation to split logical addresses into 8-bit Page Numbers (p) and 7-bit Page Offsets (d), lookup corresponding 5-bit Frame Numbers (f) via a resident Page Table, and output both decimal integers and binary bitstring representations.
- Part B (Dynamic Multi-Process Paging Scheme): Construct a complete paging simulator managing physical RAM frames via a dynamic Free Frame Pool. Dynamically allocate physical frames to incoming process pages until RAM is completely saturated. Upon exhaustion, display individual per-process Page Tables and the global system-wide Inverted Frame Allocation Table.

**Source:** [`exp9_paging.c`](exp9_paging.c)

```bash
gcc exp9_paging.c -o exp9_paging
./exp9_paging
```

**Input:** the data from the lab manual is built into the program, so no input is needed.

## Output

```
========================================================================================
  PART A: LOGICAL TO PHYSICAL ADDRESS TRANSLATION
  Parameters: Logical Address Space = 32 KB (15 bits) | Physical Address Space = 4 KB (12 bits)
              Page Size = 128 Bytes (7-bit offset)    | Pages = 256 (8 bits) | Frames = 32 (5 bits)
========================================================================================

Sample Page Table Entries (Subset of active pages):
----------------------------------------------------------------------------------------
  Page Number (p)   Binary (8-bit)   Frame Number (f)   Binary (5-bit)   Status
----------------------------------------------------------------------------------------
  0                 00000000         4                  00100            Valid
  1                 00000001         7                  00111            Valid
  2                 00000010         1                  00001            Valid
  5                 00000101         15                 01111            Valid
  8                 00001000         2                  00010            Valid
  12                00001100         28                 11100            Valid
  19                00010011         9                  01001            Valid
  25                00011001         14                 01110            Valid
  42                00101010         3                  00011            Valid
  64                01000000         21                 10101            Valid
  100               01100100         18                 10010            Valid
  150               10010110         30                 11110            Valid
  200               11001000         11                 01011            Valid
  255               11111111         25                 11001            Valid
----------------------------------------------------------------------------------------

Address Translation Results:
========================================================================================
  Logical Address   Binary (p | d)        Page#  Offset  Frame#  Physical Addr  Binary (f | d)
----------------------------------------------------------------------------------------
  0                 [00000000|0000000]  0      0       4       512            [00100|0000000]
  150               [00000001|0010110]  1      22      7       918            [00111|0010110]
  380               [00000010|1111100]  2      124     1       252            [00001|1111100]
  650               [00000101|0001010]  5      10      15      1930           [01111|0001010]
  1035              [00001000|0001011]  8      11      2       267            [00010|0001011]
  2450              [00010011|0010010]  19     18      9       1170           [01001|0010010]
  5425              [00101010|0110001]  42     49      3       433            [00011|0110001]
  12900             [01100100|1100100]  100    100     18      2404           [10010|1100100]
  32700             [11111111|0111100]  255    60      25      3260           [11001|0111100]
========================================================================================

========================================================================================
  PART B: DYNAMIC PAGING SCHEME & FREE FRAME ALLOCATOR
========================================================================================
Configuration:
  Physical RAM Size : 32 KB
  Page / Frame Size : 4 KB
  Total Frames in RAM: 8 frames (Frames 0 to 7)

Process Workload Specifications:
  Process P1: Size = 12 KB -> Requires 3 pages
  Process P2: Size = 16 KB -> Requires 4 pages
  Process P3: Size =  8 KB -> Requires 2 pages

Dynamic Free Frame Allocation Sequence:
----------------------------------------------------------------------------------------
  Allocated Physical Frame #0 -> Process P1 (Page 0) [Frames remaining: 7]
  Allocated Physical Frame #1 -> Process P1 (Page 1) [Frames remaining: 6]
  Allocated Physical Frame #2 -> Process P1 (Page 2) [Frames remaining: 5]
  Allocated Physical Frame #3 -> Process P2 (Page 0) [Frames remaining: 4]
  Allocated Physical Frame #4 -> Process P2 (Page 1) [Frames remaining: 3]
  Allocated Physical Frame #5 -> Process P2 (Page 2) [Frames remaining: 2]
  Allocated Physical Frame #6 -> Process P2 (Page 3) [Frames remaining: 1]
  Allocated Physical Frame #7 -> Process P3 (Page 0) [Frames remaining: 0]
  [ALERT] RAM FRAMES EXHAUSTED! Process P3 Page 1 cannot be accommodated.
----------------------------------------------------------------------------------------

Individual Process Page Tables:
----------------------------------------------------------------------------------------
  [Page Table for Process P1]:
    Page 0 ---> Frame #0 (Resident in RAM)
    Page 1 ---> Frame #1 (Resident in RAM)
    Page 2 ---> Frame #2 (Resident in RAM)
  [Page Table for Process P2]:
    Page 0 ---> Frame #3 (Resident in RAM)
    Page 1 ---> Frame #4 (Resident in RAM)
    Page 2 ---> Frame #5 (Resident in RAM)
    Page 3 ---> Frame #6 (Resident in RAM)
  [Page Table for Process P3]:
    Page 0 ---> Frame #7 (Resident in RAM)
    Page 1 ---> NOT ALLOCATED (Page Table Entry Invalid / Swapped)
----------------------------------------------------------------------------------------

System Inverted Frame Allocation Table (Physical RAM Status):
========================================================================================
  Frame Number   State        Loaded Process   Loaded Page   Memory Address Range
----------------------------------------------------------------------------------------
  Frame #0       ALLOCATED    Process P1       Page #0       0x00000 - 0x00FFF
  Frame #1       ALLOCATED    Process P1       Page #1       0x01000 - 0x01FFF
  Frame #2       ALLOCATED    Process P1       Page #2       0x02000 - 0x02FFF
  Frame #3       ALLOCATED    Process P2       Page #0       0x03000 - 0x03FFF
  Frame #4       ALLOCATED    Process P2       Page #1       0x04000 - 0x04FFF
  Frame #5       ALLOCATED    Process P2       Page #2       0x05000 - 0x05FFF
  Frame #6       ALLOCATED    Process P2       Page #3       0x06000 - 0x06FFF
  Frame #7       ALLOCATED    Process P3       Page #0       0x07000 - 0x07FFF
========================================================================================
```
