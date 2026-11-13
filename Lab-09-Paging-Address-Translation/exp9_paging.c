/*
 * ==============================================================================
 * OPERATING SYSTEMS LABORATORY (CS-502) - EXPERIMENT 9
 * Student Name   : Aradhya Jain
 * Scholar Number : 24112011111
 * Section        : CSE-1
 * Semester       : V Semester, B.Tech CSE
 * Institution    : Maulana Azad National Institute of Technology (MANIT) Bhopal
 * ==============================================================================
 * AIM: Logical to Physical Address Translation & Dynamic Paging Scheme Simulation
 *
 * SPECIFICATION:
 *   - Part A: Logical address (15 bits, 32 KB) to physical address (12 bits, 4 KB)
 *             Page size = 128 bytes (7-bit offset).
 *             Page Table: 256 pages (8-bit page #) -> 32 frames (5-bit frame #).
 *             Print both decimal and binary representations.
 *   - Part B: Dynamic Paging Scheme with Free Frame Allocator, Multi-process
 *             allocation, Inverted Page/Frame Table status display.
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void to_binary_str(unsigned int val, int bits, char *buf) {
    for (int i = bits - 1; i >= 0; i--) {
        buf[bits - 1 - i] = ((val >> i) & 1) ? '1' : '0';
    }
    buf[bits] = '\0';
}

void run_part_a() {
    printf("========================================================================================\n");
    printf("  PART A: LOGICAL TO PHYSICAL ADDRESS TRANSLATION\n");
    printf("  Parameters: Logical Address Space = 32 KB (15 bits) | Physical Address Space = 4 KB (12 bits)\n");
    printf("              Page Size = 128 Bytes (7-bit offset)    | Pages = 256 (8 bits) | Frames = 32 (5 bits)\n");
    printf("========================================================================================\n\n");

    #define NUM_PAGES 256
    #define NUM_FRAMES 32
    #define OFFSET_BITS 7
    #define PAGE_BITS 8
    #define FRAME_BITS 5
    #define PAGE_SIZE 128

    // Initialize Page Table: map selected pages to frames, others marked invalid (-1)
    int page_table[NUM_PAGES];
    bool valid_bit[NUM_PAGES];
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i] = -1;
        valid_bit[i] = false;
    }

    // Pre-populate realistic page mappings for test scenario
    int sample_pages[] = {0, 1, 2, 5, 8, 12, 19, 25, 42, 64, 100, 150, 200, 255};
    int sample_frames[] = {4, 7, 1, 15, 2, 28, 9, 14, 3, 21, 18, 30, 11, 25};
    int num_samples = sizeof(sample_pages) / sizeof(sample_pages[0]);

    for (int i = 0; i < num_samples; i++) {
        page_table[sample_pages[i]] = sample_frames[i];
        valid_bit[sample_pages[i]] = true;
    }

    printf("Sample Page Table Entries (Subset of active pages):\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Page Number (p)   Binary (8-bit)   Frame Number (f)   Binary (5-bit)   Status\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < num_samples; i++) {
        char p_bin[10], f_bin[10];
        to_binary_str(sample_pages[i], PAGE_BITS, p_bin);
        to_binary_str(sample_frames[i], FRAME_BITS, f_bin);
        printf("  %-17d %-16s %-18d %-16s Valid\n", sample_pages[i], p_bin, sample_frames[i], f_bin);
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    // Address translation test cases
    unsigned int test_addresses[] = {
        0,       // Page 0, Offset 0
        150,     // Page 1, Offset 22
        380,     // Page 2, Offset 124
        650,     // Page 5, Offset 10
        1035,    // Page 8, Offset 11
        2450,    // Page 19, Offset 18
        5425,    // Page 42, Offset 49
        12900,   // Page 100, Offset 100
        32700    // Page 255, Offset 60
    };
    int n_tests = sizeof(test_addresses) / sizeof(test_addresses[0]);

    printf("Address Translation Results:\n");
    printf("========================================================================================\n");
    printf("  Logical Address   Binary (p | d)        Page#  Offset  Frame#  Physical Addr  Binary (f | d)\n");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < n_tests; i++) {
        unsigned int la = test_addresses[i];
        unsigned int p = (la >> OFFSET_BITS) & 0xFF;
        unsigned int d = la & 0x7F;

        char la_bin[20], p_bin[10], d_bin[10];
        to_binary_str(la, 15, la_bin);
        to_binary_str(p, 8, p_bin);
        to_binary_str(d, 7, d_bin);

        if (valid_bit[p]) {
            int f = page_table[p];
            unsigned int pa = (f << OFFSET_BITS) | d;
            char pa_bin[20], f_bin[10];
            to_binary_str(pa, 12, pa_bin);
            to_binary_str(f, 5, f_bin);

            printf("  %-17u [%s|%s]  %-6u %-7u %-7d %-14u [%s|%s]\n",
                   la, p_bin, d_bin, p, d, f, pa, f_bin, d_bin);
        } else {
            printf("  %-17u [%s|%s]  %-6u %-7u [PAGE FAULT / INVALID ENTRY]\n",
                   la, p_bin, d_bin, p, d);
        }
    }
    printf("========================================================================================\n\n");
}

void run_part_b() {
    printf("========================================================================================\n");
    printf("  PART B: DYNAMIC PAGING SCHEME & FREE FRAME ALLOCATOR\n");
    printf("========================================================================================\n");

    int ram_size_kb = 32;     // 32 KB RAM
    int page_size_kb = 4;     // 4 KB Page/Frame size
    int total_frames = ram_size_kb / page_size_kb; // 8 Frames

    printf("Configuration:\n");
    printf("  Physical RAM Size : %d KB\n", ram_size_kb);
    printf("  Page / Frame Size : %d KB\n", page_size_kb);
    printf("  Total Frames in RAM: %d frames (Frames 0 to %d)\n\n", total_frames, total_frames - 1);

    // Frame tracking structures
    typedef struct {
        int frame_no;
        bool is_free;
        int proc_id;
        int page_no;
    } Frame;

    Frame frame_table[total_frames];
    for (int i = 0; i < total_frames; i++) {
        frame_table[i].frame_no = i;
        frame_table[i].is_free = true;
        frame_table[i].proc_id = -1;
        frame_table[i].page_no = -1;
    }

    // Processes
    typedef struct {
        int proc_id;
        int size_kb;
        int num_pages;
        int page_table[16];
    } Process;

    int num_procs = 3;
    Process procs[3] = {
        {1, 12, 3, {-1, -1, -1}},  // P1 needs 3 pages (12 KB)
        {2, 16, 4, {-1, -1, -1, -1}}, // P2 needs 4 pages (16 KB)
        {3, 8,  2, {-1, -1}}       // P3 needs 2 pages (8 KB) -> will partially fill or test frame limit
    };

    printf("Process Workload Specifications:\n");
    for (int i = 0; i < num_procs; i++) {
        printf("  Process P%d: Size = %2d KB -> Requires %d pages\n",
               procs[i].proc_id, procs[i].size_kb, procs[i].num_pages);
    }
    printf("\nDynamic Free Frame Allocation Sequence:\n");
    printf("----------------------------------------------------------------------------------------\n");

    int free_frames_remaining = total_frames;
    int next_free_idx = 0;

    // Simulate allocating pages to free frames
    for (int p_idx = 0; p_idx < num_procs; p_idx++) {
        for (int pg = 0; pg < procs[p_idx].num_pages; pg++) {
            if (free_frames_remaining == 0) {
                printf("  [ALERT] RAM FRAMES EXHAUSTED! Process P%d Page %d cannot be accommodated.\n",
                       procs[p_idx].proc_id, pg);
                goto allocation_done;
            }

            // Find next free frame
            while (next_free_idx < total_frames && !frame_table[next_free_idx].is_free) {
                next_free_idx++;
            }

            if (next_free_idx < total_frames) {
                frame_table[next_free_idx].is_free = false;
                frame_table[next_free_idx].proc_id = procs[p_idx].proc_id;
                frame_table[next_free_idx].page_no = pg;
                procs[p_idx].page_table[pg] = next_free_idx;

                printf("  Allocated Physical Frame #%d -> Process P%d (Page %d) [Frames remaining: %d]\n",
                       next_free_idx, procs[p_idx].proc_id, pg, free_frames_remaining - 1);

                free_frames_remaining--;
                next_free_idx++;
            }
        }
    }

allocation_done:
    printf("----------------------------------------------------------------------------------------\n\n");

    // Print Individual Process Page Tables
    printf("Individual Process Page Tables:\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < num_procs; i++) {
        printf("  [Page Table for Process P%d]:\n", procs[i].proc_id);
        for (int pg = 0; pg < procs[i].num_pages; pg++) {
            if (procs[i].page_table[pg] != -1) {
                printf("    Page %d ---> Frame #%d (Resident in RAM)\n", pg, procs[i].page_table[pg]);
            } else {
                printf("    Page %d ---> NOT ALLOCATED (Page Table Entry Invalid / Swapped)\n", pg);
            }
        }
    }
    printf("----------------------------------------------------------------------------------------\n\n");

    // Print System Inverted Frame Allocation Table
    printf("System Inverted Frame Allocation Table (Physical RAM Status):\n");
    printf("========================================================================================\n");
    printf("  Frame Number   State        Loaded Process   Loaded Page   Memory Address Range\n");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < total_frames; i++) {
        int start_addr = i * page_size_kb * 1024;
        int end_addr = start_addr + (page_size_kb * 1024) - 1;
        if (!frame_table[i].is_free) {
            printf("  Frame #%-7d ALLOCATED    Process P%-7d Page #%-7d 0x%05X - 0x%05X\n",
                   i, frame_table[i].proc_id, frame_table[i].page_no, start_addr, end_addr);
        } else {
            printf("  Frame #%-7d FREE         ---              ---           0x%05X - 0x%05X\n",
                   i, start_addr, end_addr);
        }
    }
    printf("========================================================================================\n");
}

int main() {
    run_part_a();
    run_part_b();
    return 0;
}
