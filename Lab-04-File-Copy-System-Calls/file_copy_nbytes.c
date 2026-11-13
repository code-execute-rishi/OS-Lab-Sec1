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
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define CHUNK_SIZE 1024

int main(int argc, char *argv[]) {
    char src_file[256];
    char dest_file[256];
    long bytes_requested = 0;

    printf("========================================================\n");
    printf("   OS Lab 4: Low-Level I/O Copy (open, read, write)    \n");
    printf("   Student: Aradhya Jain | Scholar No: 24112011111     \n");
    printf("========================================================\n");

    if (argc >= 4) {
        strncpy(src_file, argv[1], sizeof(src_file) - 1);
        src_file[sizeof(src_file) - 1] = '\0';
        strncpy(dest_file, argv[2], sizeof(dest_file) - 1);
        dest_file[sizeof(dest_file) - 1] = '\0';
        bytes_requested = atol(argv[3]);
    } else {
        printf("Enter source file path      : ");
        if (scanf("%255s", src_file) != 1) {
            fprintf(stderr, "Error: Failed to read source file name.\n");
            return 1;
        }

        printf("Enter destination file path : ");
        if (scanf("%255s", dest_file) != 1) {
            fprintf(stderr, "Error: Failed to read destination file name.\n");
            return 1;
        }

        printf("Enter number of bytes to copy: ");
        if (scanf("%ld", &bytes_requested) != 1) {
            fprintf(stderr, "Error: Invalid byte count input.\n");
            return 1;
        }
    }

    if (bytes_requested <= 0) {
        fprintf(stderr, "Error: Number of bytes to copy must be positive (> 0).\n");
        return 1;
    }

    /* 1. Open source file in Read-Only mode */
    int src_fd = open(src_file, O_RDONLY);
    if (src_fd < 0) {
        fprintf(stderr, "System Call Error: open(\"%s\", O_RDONLY) failed: %s\n",
                src_file, strerror(errno));
        return 2;
    }

    /* Get file size via fstat to inform the user */
    struct stat st;
    if (fstat(src_fd, &st) == 0) {
        printf("\n[INFO] Source file '%s' size: %lld bytes\n", src_file, (long long)st.st_size);
        if (bytes_requested > st.st_size) {
            printf("[WARN] Requested %ld bytes exceeds file size (%lld bytes). Will copy available bytes.\n",
                   bytes_requested, (long long)st.st_size);
        }
    }

    /* 2. Open / Create destination file (Write-Only, Create if missing, Truncate if exists, 0644 perms) */
    int dest_fd = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        fprintf(stderr, "System Call Error: open(\"%s\", O_WRONLY|O_CREAT|O_TRUNC) failed: %s\n",
                dest_file, strerror(errno));
        close(src_fd);
        return 3;
    }

    char buffer[CHUNK_SIZE];
    long total_copied = 0;
    long remaining = bytes_requested;

    printf("[PROGRESS] Starting byte-level transfer...\n");

    /* 3. Read and Write loop */
    while (remaining > 0) {
        size_t to_read = (remaining > CHUNK_SIZE) ? CHUNK_SIZE : (size_t)remaining;
        ssize_t bytes_read = read(src_fd, buffer, to_read);

        if (bytes_read < 0) {
            fprintf(stderr, "\nSystem Call Error: read() encountered an error: %s\n", strerror(errno));
            break;
        }

        if (bytes_read == 0) {
            /* EOF reached before requested byte count */
            printf("\n[NOTE] End-Of-File (EOF) reached at %ld bytes.\n", total_copied);
            break;
        }

        /* Write exactly what was read */
        ssize_t written_now = 0;
        while (written_now < bytes_read) {
            ssize_t res = write(dest_fd, buffer + written_now, bytes_read - written_now);
            if (res < 0) {
                fprintf(stderr, "\nSystem Call Error: write() failed: %s\n", strerror(errno));
                close(src_fd);
                close(dest_fd);
                return 4;
            }
            written_now += res;
        }

        total_copied += bytes_read;
        remaining -= bytes_read;
    }

    /* 4. Close both file descriptors */
    close(src_fd);
    close(dest_fd);

    printf("--------------------------------------------------------\n");
    printf("Transfer Successful!\n");
    printf("  Target Requested : %ld bytes\n", bytes_requested);
    printf("  Actual Transferred: %ld bytes\n", total_copied);
    printf("  Source File      : %s\n", src_file);
    printf("  Destination File : %s\n", dest_file);
    printf("========================================================\n");

    return 0;
}
