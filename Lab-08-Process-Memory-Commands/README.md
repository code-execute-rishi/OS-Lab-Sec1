# Experiment 8: Process & Main Memory Related Linux Commands Diagnostics

**Aim:** To explore, execute, and rigorously analyze system monitoring and resource diagnostics commands in UNIX/Linux across two core subsystems:
- Part A (Process Management Commands): Study and demonstrate UNIX job control mechanisms (bg, fg, jobs), real-time process monitoring (top), priority scheduling manipulation (nice, renice), and persistent detached process execution immune to terminal hangups (nohup).
- Part B (Main Memory Diagnostics Commands): Investigate methods for determining the hardware/virtual memory page size (getconf PAGESIZE), analyze physical and swap memory allocation via free, inspect kernel virtual memory internals via the /proc/meminfo pseudo-filesystem, and parse virtual memory activity via vmstat.

Commands and outputs below are copied from the lab file (Ubuntu 22.04 VM). The theory and explanation tables are in the lab file PDF.

### 2.1 UNIX Job Control Architecture (bg, fg, jobs)

```
aradhya@aradhya-os-vm:~$ sleep 300
^Z
[1]+ Stopped                  sleep 300
aradhya@aradhya-os-vm:~$ jobs -l
[1]+ 4105 Stopped                  sleep 300
aradhya@aradhya-os-vm:~$ bg %1
[1]+ sleep 300 &
aradhya@aradhya-os-vm:~$ jobs -l
[1]+ 4105 Running                  sleep 300 &
aradhya@aradhya-os-vm:~$ fg %1
sleep 300
^C
```

### 2.2 Immunity to Terminal Detachment: The nohup Command

```
aradhya@aradhya-os-vm:~$ nohup ./data_cruncher.sh &
[1] 5240
nohup: ignoring input and appending output to 'nohup.out'
aradhya@aradhya-os-vm:~$ disown -h %1
aradhya@aradhya-os-vm:~$ exit
```

### 2.4 SCHEDULING PRIORITY CONTROL: THE 'NICE' AND 'RENICE' COMMANDS

```
aradhya@aradhya-os-vm:~$ nice -n 10 ./background_worker &
[1] 6120
aradhya@aradhya-os-vm:~$ ps -o pid,ni,pri,comm -p 6120
    PID NI PRI COMMAND
   6120 10 30 background_worker
aradhya@aradhya-os-vm:~$ sudo renice -n 5 -p 6120
6120 (process ID) old priority 10, new priority 5
aradhya@aradhya-os-vm:~$ ps -o pid,ni,pri,comm -p 6120
    PID NI PRI COMMAND
   6120   5 25 background_worker
```

### 3.2 Comprehensive Analysis of the 'free' Command Output

```
aradhya@aradhya-os-vm:~$ free -h --wide
               total        used        free             shared       buffers         cache     available
Mem:           7.7Gi       2.1Gi       3.2Gi              142Mi         185Mi         2.2Gi         5.2Gi
Swap:          2.0Gi          0B       2.0Gi
```

### 3.4 VIRTUAL MEMORY ACTIVITY ANALYSIS VIA 'VMSTAT'

```
aradhya@aradhya-os-vm:~$ vmstat 1 4
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r b    swpd   free   buff cache    si   so    bi    bo   in   cs us sy id wa st
 1 0       0 335412 18942 225410    0    0    12    45 150 320 2 1 97 0 0
 0 0       0 335388 18942 225410    0    0     0     0 210 440 1 1 98 0 0
 2 0       0 335210 18942 225410    0    0     0   128 285 510 5 2 93 0 0
 0 0       0 335210 18942 225410    0    0     0     0 180 390 1 0 99 0 0
```

### 3.1 Determining Virtual Memory Page Size

- Shell Utility Method: `getconf PAGESIZE` or `getconf PAGE_SIZE` queries system configuration limits (returns 4096 bytes on x86_64, or 16384 bytes on Apple Silicon ARM64).
- C Programmatic System Call: `sysconf(_SC_PAGESIZE)` or the POSIX library function `getpagesize()`.
- Procfs Inspection: Inspecting `/proc/self/smaps` reveals `KernelPageSize: 4 kB` and `MMUPageSize: 4 kB`.
