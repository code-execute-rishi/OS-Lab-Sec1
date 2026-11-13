/**
 * ============================================================================
 * Operating Systems Laboratory (CS-502) - Experiment 4
 * Student Name  : Aradhya Jain
 * Scholar No    : 24112011111
 * Section       : CSE-1
 *
 * Program: Interactive File Copy of N Bytes using open(), read(), write()
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char src[256], dest[256];
    long n;

    printf("========================================================\n");
    printf("   OS Lab 4: Low-Level I/O Copy (open, read, write)    \n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111     \n");
    printf("========================================================\n");

    // get source, destination and N from arguments or from the user
    if (argc >= 4) {
        strcpy(src, argv[1]);
        strcpy(dest, argv[2]);
        n = atol(argv[3]);
    } else {
        printf("Enter source file path      : ");
        if (scanf("%255s", src) != 1) {
            fprintf(stderr, "Error: Failed to read source file name.\n");
            return 1;
        }
        printf("Enter destination file path : ");
        if (scanf("%255s", dest) != 1) {
            fprintf(stderr, "Error: Failed to read destination file name.\n");
            return 1;
        }
        printf("Enter number of bytes to copy: ");
        if (scanf("%ld", &n) != 1) {
            fprintf(stderr, "Error: Invalid byte count input.\n");
            return 1;
        }
    }

    if (n <= 0) {
        fprintf(stderr, "Error: Number of bytes to copy must be positive (> 0).\n");
        return 1;
    }

    // 1. open source for reading
    int in = open(src, O_RDONLY);
    if (in < 0) {
        fprintf(stderr, "System Call Error: open(\"%s\", O_RDONLY) failed: %s\n", src, strerror(errno));
        return 2;
    }

    // show file size (fstat)
    struct stat st;
    fstat(in, &st);
    long size = st.st_size;
    printf("\n[INFO] Source file '%s' size: %ld bytes\n", src, size);
    if (n > size)
        printf("[WARN] Requested %ld bytes exceeds file size (%ld bytes). Will copy available bytes.\n", n, size);

    // 2. open/create destination for writing (truncate old content)
    int out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        fprintf(stderr, "System Call Error: open(\"%s\", O_WRONLY|O_CREAT|O_TRUNC) failed: %s\n", dest, strerror(errno));
        return 3;
    }

    printf("[PROGRESS] Starting byte-level transfer...\n");

    // 3. copy one byte at a time: read 1, write 1
    char c;
    long copied = 0;
    while (copied < n) {
        if (read(in, &c, 1) <= 0) {  // 0 = end of file
            printf("\n[NOTE] End-Of-File (EOF) reached at %ld bytes.\n", copied);
            break;
        }
        write(out, &c, 1);
        copied++;
    }

    // 4. close both files
    close(in);
    close(out);

    printf("--------------------------------------------------------\n");
    printf("Transfer Successful!\n");
    printf("  Target Requested : %ld bytes\n", n);
    printf("  Actual Transferred: %ld bytes\n", copied);
    printf("  Source File      : %s\n", src);
    printf("  Destination File : %s\n", dest);
    printf("========================================================\n");
    return 0;
}
