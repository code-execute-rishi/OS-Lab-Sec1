# Experiment 3: VIM Modal Editor & Linux File Access Permissions / Ownership Management

**Aim:**
1. Part A: To master the modal architecture of the VIM Editor (Vi IMproved), understanding the functional boundaries and transitions between Command/Normal Mode, Insert Mode, and Visual/Edit Mode, and tabulate essential key combinations and Ex commands.
2. Part B: To investigate Linux File Access Permissions and Ownership, mastering the chmod command in both Relative (Symbolic) Mode and Absolute (Octal) Mode, and managing user/group ownership using the chown command.

Commands and outputs below are copied from the lab file (Ubuntu 22.04 VM). The theory and explanation tables are in the lab file PDF.

### 2.3 Terminal Simulation of VIM Session Workflow

```
$ vim test_script.c
~
~   1 #include <stdio.h>
~   2 int main() {
~   3     printf("OS Lab Experiment 3 - Aradhya Jain\n");
~   4     return 0;
~   5 }
~
"test_script.c" [New] 5L, 94B written
$ cat test_script.c
#include <stdio.h>
int main() {
    printf("OS Lab Experiment 3 - Aradhya Jain\n");
    return 0;
}
```

### 3.1 The chmod Command: Changing File Access Permissions

```
aradhya@aradhya-os-vm:~$ touch sample.txt && ls -l sample.txt
-rw-r--r-- 1 aradhya aradhya 0 Oct 7 20:45 sample.txt

aradhya@aradhya-os-vm:~$ chmod u+x,g+w,o-r sample.txt && ls -l sample.txt
-rwxr--w--- 1 aradhya aradhya 0 Oct 7 20:45 sample.txt

aradhya@aradhya-os-vm:~$ chmod 755 sample.txt && ls -l sample.txt
-rwxr-xr-x 1 aradhya aradhya 0 Oct 7 20:45 sample.txt

aradhya@aradhya-os-vm:~$ chmod 600 sample.txt && ls -l sample.txt
-rw------- 1 aradhya aradhya 0 Oct 7 20:45 sample.txt
```

### 3.2 The chown Command: Modifying File Ownership & Group

```
aradhya@aradhya-os-vm:~$ ls -l /tmp/project_share.txt
-rw-r--r-- 1 aradhya aradhya 128 Oct 7 20:50 /tmp/project_share.txt

aradhya@aradhya-os-vm:~$ sudo chown root:adm /tmp/project_share.txt
aradhya@aradhya-os-vm:~$ ls -l /tmp/project_share.txt
-rw-r--r-- 1 root adm 128 Oct 7 20:50 /tmp/project_share.txt

aradhya@aradhya-os-vm:~$ sudo chown -v aradhya:aradhya /tmp/project_share.txt
changed ownership of '/tmp/project_share.txt' from root:adm to aradhya:aradhya
```
