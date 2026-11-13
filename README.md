# Operating Systems Lab (CS-502) — Section 1

Lab programs for the Operating Systems Laboratory (CS-502), B.Tech V Semester, Maulana Azad National Institute of Technology (MANIT), Bhopal.

| Exp | Topic | Files |
|-----|-------|-------|
| [01](Lab-01-Ubuntu-Linux-Commands) | Installation of Ubuntu with VirtualBox & basic Linux commands | commands only |
| [02](Lab-02-Directory-Structure-Kernel-Modules) | UNIX/Linux directory structure, kernel modules & hardware diagnostics | commands only |
| [03](Lab-03-Vim-File-Permissions) | VIM editor modes & file access permissions / ownership | commands only |
| [04](Lab-04-File-Copy-System-Calls) | Low-level system calls `open()`, `read()`, `write()`: copy N bytes | `file_copy_nbytes.c` |
| [05](Lab-05-Fork-Exec-Wait) | Process management: `fork()`, `exec()`, `wait()` | `fork_n_children.c`, `parent_wait_child.c` |
| [06](Lab-06-CPU-Scheduling) | CPU scheduling: FCFS, SJF, SRTF, RR, Priority (NP & P), RR quantum sweep | `cpu_scheduler.c` |
| [07](Lab-07-MLFQ-Scheduling) | Multilevel Feedback Queue (MLFQ) scheduling | `exp7_mlfq.c` |
| [08](Lab-08-Process-Memory-Commands) | Process & main memory Linux commands | commands only |
| [09](Lab-09-Paging-Address-Translation) | Logical to physical address translation & paging | `exp9_paging.c` |
| [10](Lab-10-Fixed-Partitioning) | Fixed partitioning: First Fit & Best Fit | `exp10_fixed_partitioning.c` |
| [11](Lab-11-Variable-Partitioning-Worst-Fit) | Variable partitioning: Worst Fit & external fragmentation | `exp11_variable_partitioning.c` |
| [12](Lab-12-Bankers-Producer-Consumer) | Banker's algorithm & Producer-Consumer | `exp12_banker_prodcons.c` |
| [13](Lab-13-Semaphores-RAG) | Semaphores & Resource Allocation Graph deadlock detection | `exp13_semaphore_rag.c` |

## Build and run

Each lab folder has its own README with the aim, the commands to compile and run, the input, and the output.

```bash
cd Lab-07-MLFQ-Scheduling
gcc exp7_mlfq.c -o exp7_mlfq
./exp7_mlfq
```

The programs use POSIX system calls, so they run on Linux and macOS. On Windows, use WSL.
