# Experiment 1: Installation of Ubuntu with VirtualBox & Linux Commands Exploration

**Aim:**
1. Part A: To configure and install the Ubuntu Linux Operating System inside a guest virtual machine environment using Oracle VM VirtualBox hypervisor.
2. Part B: To study, execute, and analyze the syntax, operational behavior, options, and terminal outputs of 17 foundational Linux shell commands: man, ls, bc, cat, cmp, cp, date, df, echo, file, mkdir, mv, pwd, ps, rm, rmdir, and wc.

Commands and outputs below are copied from the lab file (Ubuntu 22.04 VM). The theory and explanation tables are in the lab file PDF.

### 2.4 Post-Installation Verification & Environment Diagnostics

```
aradhya@aradhya-os-vm:~$ uname -a
Linux aradhya-os-vm 5.15.0-88-generic #98-Ubuntu SMP Mon Oct 2 15:18:56 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux

aradhya@aradhya-os-vm:~$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 22.04.3 LTS
Release:        22.04
Codename:       jammy

aradhya@aradhya-os-vm:~$ systemd-detect-virt
oracle

aradhya@aradhya-os-vm:~$ free -h
               total        used              free       shared   buff/cache     available
Mem:           3.9Gi       850Mi             2.4Gi         28Mi        714Mi         2.8Gi
Swap:          2.0Gi          0B             2.0Gi
```

### Command 1: man — System Reference Manual Interface

```
aradhya@aradhya-os-vm:~$ man -f ls
ls (1)               - list directory contents

aradhya@aradhya-os-vm:~$ man 2 open | head -n 12
OPEN(2)                     Linux Programmer's Manual                               OPEN(2)

NAME
        open, openat, creat - open and possibly create a file

SYNOPSIS
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

        int open(const char *pathname, int flags);
        int open(const char *pathname, int flags, mode_t mode);
```

### Command 2: ls — List Directory Contents

```
aradhya@aradhya-os-vm:~$ ls -la /var/log/syslog* | head -n 3
-rw-r----- 1 syslog adm 245192 Oct 7 20:15 /var/log/syslog
-rw-r----- 1 syslog adm 812401 Oct 6 23:59 /var/log/syslog.1
```

### Command 3: bc — Arbitrary Precision Calculator Language

```
aradhya@aradhya-os-vm:~$ echo "scale=6; 22/7; 4 * a(1)" | bc -l
3.142857
3.14159265358979323844
```

### Command 4: cat — Concatenate and Print Files

```
aradhya@aradhya-os-vm:~$ cat << 'EOF' > lab_demo.txt
OS Laboratory CS-502
Student: Aradhya Jain
EOF
aradhya@aradhya-os-vm:~$ cat -n lab_demo.txt
     1 OS Laboratory CS-502
     2 Student: Aradhya Jain
```

### Command 5: cmp — Byte-by-Byte File Comparison

```
aradhya@aradhya-os-vm:~$ echo "MANIT Bhopal" > f1.txt && echo "MANIT Bharat" > f2.txt
aradhya@aradhya-os-vm:~$ cmp f1.txt f2.txt
f1.txt f2.txt differ: byte 9, line 1
aradhya@aradhya-os-vm:~$ echo $?
1
```

### Command 6: cp — Copy Files and Directories

```
aradhya@aradhya-os-vm:~$ cp -v lab_demo.txt lab_demo_backup.txt
'lab_demo.txt' -> 'lab_demo_backup.txt'
```

### Command 7: date — Display or Set System Date and Time

```
aradhya@aradhya-os-vm:~$ date +"%A, %d %B %Y | Time: %T %Z | Epoch: %s"
Wednesday, 07 October 2026 | Time: 20:30:15 IST | Epoch: 1791385215
```

### Command 8: df — Report File System Disk Space Usage

```
aradhya@aradhya-os-vm:~$ df -hT / /boot/efi
Filesystem     Type   Size Used Avail Use% Mounted on
/dev/sda2      ext4    24G 7.8G    15G 35% /
/dev/sda1      vfat   511M 6.1M 505M     2% /boot/efi
```

### Command 9: echo — Display a Line of Text

```
aradhya@aradhya-os-vm:~$ echo -e "User: $USER\nShell: $SHELL\nKernel: $(uname -r)"
User:   aradhya
Shell: /bin/bash
Kernel: 5.15.0-88-generic
```

### Command 10: file — Determine File Type via Magic Numbers

```
aradhya@aradhya-os-vm:~$ file /bin/bash /etc/passwd /boot/vmlinuz-5.15.0-88-generic
/bin/bash:                         ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked
/etc/passwd:                       ASCII text
/boot/vmlinuz-5.15.0-88-generic:    Linux kernel x86 boot executable bzImage, version 5.15.0-88-generic
```

### Command 11: mkdir — Make Directories

```
aradhya@aradhya-os-vm:~$ mkdir -p -v ~/os_lab/experiment1/test_dir
mkdir: created directory '/home/aradhya/os_lab'
mkdir: created directory '/home/aradhya/os_lab/experiment1'
mkdir: created directory '/home/aradhya/os_lab/experiment1/test_dir'
```

### Command 12: mv — Move (Rename) Files and Directories

```
aradhya@aradhya-os-vm:~$ mv -v lab_demo_backup.txt ~/os_lab/experiment1/archived_demo.txt
renamed 'lab_demo_backup.txt' -> '/home/aradhya/os_lab/experiment1/archived_demo.txt'
```

### Command 13: pwd — Print Working Directory

```
aradhya@aradhya-os-vm:~$ cd ~/os_lab/experiment1 && pwd -P
/home/aradhya/os_lab/experiment1
```

### Command 14: ps — Report Process Status Snapshot

```
aradhya@aradhya-os-vm:~$ ps -o pid,ppid,stat,%cpu,%mem,comm | head -n 5
    PID    PPID STAT %CPU %MEM COMMAND
   3140    3128 Ss     0.0 0.2 bash
   3520    3140 R+     0.0 0.1 ps
```

### Command 15: rm — Remove Files or Directories

```
aradhya@aradhya-os-vm:~$ rm -v f1.txt f2.txt
removed 'f1.txt'
removed 'f2.txt'
```

### Command 16: rmdir — Remove Empty Directories

```
aradhya@aradhya-os-vm:~$ rmdir -v ~/os_lab/experiment1/test_dir
rmdir: removing directory, '/home/aradhya/os_lab/experiment1/test_dir'
```

### Command 17: wc — Print Line, Word, and Byte Counts

```
aradhya@aradhya-os-vm:~$ wc lab_demo.txt
 2 6 43 lab_demo.txt
aradhya@aradhya-os-vm:~$ wc -l -w -c lab_demo.txt
Lines: 2 | Words: 6 | Bytes: 43
```
