# Experiment 4: UNIX Low-Level System Calls & C Programming (open, read, write)

**Aim:**
1. Part A: To study the low-level UNIX file I/O system calls—open(), read(), and write()—analyzing their prototypes, input parameters, bitwise flags, file descriptors, return types, and associated POSIX C library headers.
2. Part B: To develop and verify an interactive C program that copies an exact user-specified number of bytes from an existing file to a new target file using exclusively open(), read(), and write(), handling all execution edge cases (missing files, invalid byte counts, partial writes, EOF boundary conditions).

**Source:** [`file_copy_nbytes.c`](file_copy_nbytes.c)

```bash
gcc file_copy_nbytes.c -o file_copy_nbytes
./file_copy_nbytes                                   # interactive: asks for source, destination, byte count
./file_copy_nbytes <source> <destination> <bytes>    # or pass them as arguments
```

**Input file** ([`sample_input.txt`](sample_input.txt), 89 bytes)

```
The quick brown fox jumps over the lazy dog. Operating Systems Lab CS-502. MANIT Bhopal.
```

### Test Case 1: Standard byte copy (N = 44 bytes)

```
$ ./file_copy_nbytes sample_input.txt sample_output.txt 44
========================================================
   OS Lab 4: Low-Level I/O Copy (open, read, write)
   Student: Aradhya Jain | Scholar No: 24112011111
========================================================

[INFO] Source file 'sample_input.txt' size: 89 bytes
[PROGRESS] Starting byte-level transfer...
--------------------------------------------------------
Transfer Successful!
  Target Requested : 44 bytes
  Actual Transferred: 44 bytes
  Source File      : sample_input.txt
  Destination File : sample_output.txt
========================================================
$ cat sample_output.txt
The quick brown fox jumps over the lazy dog.
```

### Test Case 2: Byte count exceeds source file size (EOF handling)

```
$ ./file_copy_nbytes sample_input.txt dest_oversize.txt 500
========================================================
   OS Lab 4: Low-Level I/O Copy (open, read, write)
   Student: Aradhya Jain | Scholar No: 24112011111
========================================================

[INFO] Source file 'sample_input.txt' size: 89 bytes
[WARN] Requested 500 bytes exceeds file size (89 bytes). Will copy available bytes.
[PROGRESS] Starting byte-level transfer...

[NOTE] End-Of-File (EOF) reached at 89 bytes.
--------------------------------------------------------
Transfer Successful!
  Target Requested : 500 bytes
  Actual Transferred: 89 bytes
  Source File      : sample_input.txt
  Destination File : dest_oversize.txt
========================================================
```

### Test Case 3: Missing source file

```
$ ./file_copy_nbytes non_existent_file.txt output.txt 50
========================================================
   OS Lab 4: Low-Level I/O Copy (open, read, write)
   Student: Aradhya Jain | Scholar No: 24112011111
========================================================
System Call Error: open("non_existent_file.txt", O_RDONLY) failed: No such file or directory
$ echo $?
2
```

### Test Case 4: Byte verification

```
$ wc -c sample_input.txt sample_output.txt
      89 sample_input.txt
      44 sample_output.txt
     133 total
$ head -c 44 sample_input.txt | cmp - sample_output.txt && echo "Exact match"
Exact match
```
