/*
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 9
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * AIM: Logical to Physical Address Translation & Dynamic Paging Scheme Simulation
 *   - Part A: 15-bit logical -> 12-bit physical, page size 128 (7-bit offset)
 *   - Part B: free frame allocator, per-process page tables, frame table
 */

#include <stdio.h>

// Write the last 'bits' bits of val into buf as '0'/'1' characters
void to_binary(int val, int bits, char *buf) {
    for (int i = 0; i < bits; i++)
        buf[i] = ((val >> (bits - 1 - i)) & 1) ? '1' : '0';
    buf[bits] = '\0';
}

void part_a() {
    printf("========================================================================================\n");
    printf("  PART A: LOGICAL TO PHYSICAL ADDRESS TRANSLATION\n");
    printf("  Parameters: Logical Address Space = 32 KB (15 bits) | Physical Address Space = 4 KB (12 bits)\n");
    printf("              Page Size = 128 Bytes (7-bit offset)    | Pages = 256 (8 bits) | Frames = 32 (5 bits)\n");
    printf("========================================================================================\n\n");

    // Page table: page_table[p] = frame, -1 = invalid
    int page_table[256];
    for (int i = 0; i < 256; i++) page_table[i] = -1;

    int pages[14]  = {0, 1, 2, 5, 8, 12, 19, 25, 42, 64, 100, 150, 200, 255};
    int frames[14] = {4, 7, 1, 15, 2, 28, 9, 14, 3, 21, 18, 30, 11, 25};
    char pb[20], fb[20], db[20], ab[20];

    printf("Sample Page Table Entries (Subset of active pages):\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Page Number (p)   Binary (8-bit)   Frame Number (f)   Binary (5-bit)   Status\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < 14; i++) {
        page_table[pages[i]] = frames[i];
        to_binary(pages[i], 8, pb);
        to_binary(frames[i], 5, fb);
        printf("  %-17d %-16s %-18d %-16s Valid\n", pages[i], pb, frames[i], fb);
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    int la_list[9] = {0, 150, 380, 650, 1035, 2450, 5425, 12900, 32700};

    printf("Address Translation Results:\n");
    printf("========================================================================================\n");
    printf("  Logical Address   Binary (p | d)        Page#  Offset  Frame#  Physical Addr  Binary (f | d)\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < 9; i++) {
        int la = la_list[i];
        int p = la / 128;                 // page number = high 8 bits
        int d = la % 128;                 // offset      = low 7 bits
        to_binary(p, 8, pb);
        to_binary(d, 7, db);

        if (page_table[p] != -1) {
            int f = page_table[p];
            int pa = f * 128 + d;         // physical = frame * page size + offset
            to_binary(pa, 12, ab);
            to_binary(f, 5, fb);
            printf("  %-17d [%s|%s]  %-6d %-7d %-7d %-14d [%s|%s]\n", la, pb, db, p, d, f, pa, fb, db);
        } else {
            printf("  %-17d [%s|%s]  %-6d %-7d [PAGE FAULT / INVALID ENTRY]\n", la, pb, db, p, d);
        }
    }
    printf("========================================================================================\n\n");
}

void part_b() {
    printf("========================================================================================\n");
    printf("  PART B: DYNAMIC PAGING SCHEME & FREE FRAME ALLOCATOR\n");
    printf("========================================================================================\n");

    int ram = 32, page = 4;               // sizes in KB
    int nframes = ram / page;             // 8 frames

    printf("Configuration:\n");
    printf("  Physical RAM Size : %d KB\n", ram);
    printf("  Page / Frame Size : %d KB\n", page);
    printf("  Total Frames in RAM: %d frames (Frames 0 to %d)\n\n", nframes, nframes - 1);

    // Frame table: which process/page sits in each frame (-1 = free)
    int frame_proc[8], frame_page[8];
    for (int i = 0; i < nframes; i++) frame_proc[i] = frame_page[i] = -1;

    int size[3] = {12, 16, 8};            // process sizes in KB
    int npages[3];
    int pt[3][4];                         // page table of each process (-1 = not allocated)

    printf("Process Workload Specifications:\n");
    for (int i = 0; i < 3; i++) {
        npages[i] = size[i] / page;
        for (int pg = 0; pg < 4; pg++) pt[i][pg] = -1;
        printf("  Process P%d: Size = %2d KB -> Requires %d pages\n", i + 1, size[i], npages[i]);
    }
    printf("\nDynamic Free Frame Allocation Sequence:\n");
    printf("----------------------------------------------------------------------------------------\n");

    // Give each page the next free frame, stop when RAM is full
    int f = 0, full = 0;
    for (int i = 0; i < 3 && !full; i++) {
        for (int pg = 0; pg < npages[i]; pg++) {
            if (f == nframes) {
                printf("  [ALERT] RAM FRAMES EXHAUSTED! Process P%d Page %d cannot be accommodated.\n", i + 1, pg);
                full = 1;
                break;
            }
            frame_proc[f] = i + 1;
            frame_page[f] = pg;
            pt[i][pg] = f;
            printf("  Allocated Physical Frame #%d -> Process P%d (Page %d) [Frames remaining: %d]\n",
                   f, i + 1, pg, nframes - f - 1);
            f++;
        }
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("Individual Process Page Tables:\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < 3; i++) {
        printf("  [Page Table for Process P%d]:\n", i + 1);
        for (int pg = 0; pg < npages[i]; pg++) {
            if (pt[i][pg] != -1)
                printf("    Page %d ---> Frame #%d (Resident in RAM)\n", pg, pt[i][pg]);
            else
                printf("    Page %d ---> NOT ALLOCATED (Page Table Entry Invalid / Swapped)\n", pg);
        }
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("System Inverted Frame Allocation Table (Physical RAM Status):\n");
    printf("========================================================================================\n");
    printf("  Frame Number   State        Loaded Process   Loaded Page   Memory Address Range\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < nframes; i++) {
        int start = i * page * 1024;      // frame i starts at i * frame size
        int end = start + page * 1024 - 1;
        if (frame_proc[i] != -1)
            printf("  Frame #%-7d ALLOCATED    Process P%-7d Page #%-7d 0x%05X - 0x%05X\n",
                   i, frame_proc[i], frame_page[i], start, end);
        else
            printf("  Frame #%-7d FREE         ---              ---           0x%05X - 0x%05X\n",
                   i, start, end);
    }
    printf("========================================================================================\n");
}

int main() {
    part_a();
    part_b();
    return 0;
}
