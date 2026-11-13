# Experiment 5: Process Management System Calls (fork, exec, wait) & Process Hierarchies

**Aim:**
1. Part A: To study the theoretical architecture, prototypes, input arguments, and return types of process control system calls—fork(), exec(), and wait()—and analyze the six standard variants of the exec family (execl, execv, execle, execve, execlp, execvp).
2. Part B1: To write an interactive C program that accepts an integer N from the user and spawns exactly N child processes via fork(), printing each child's PID alongside its parent's PID.
3. Part B2: To write a C program demonstrating process synchronization wherein a parent process explicitly blocks and waits for a child process to terminate using wait(), capturing and inspecting its exit status code.

PIDs are different on every run, and the order of the `[REAPED]` lines can change.

## Part B1: Generate N child processes

**Source:** [`fork_n_children.c`](fork_n_children.c)

```bash
gcc fork_n_children.c -o fork_n_children
./fork_n_children 4        # or run ./fork_n_children and enter N when asked
```

```
$ ./fork_n_children 4
========================================================
   OS Lab 5 (b1): Generating N Child Processes
   Student: Aradhya Jain | Scholar No: 24112011111
========================================================

[ROOT PARENT] PID = 29786 is initializing spawn loop for 4 children...
----------------------------------------------------------------------
Child #    Status       Child PID      Parent PID     PPID (getppid)
----------------------------------------------------------------------
1          ACTIVE       29805          29786          29786
2          ACTIVE       29806          29786          29786
3          ACTIVE       29807          29786          29786
4          ACTIVE       29808          29786          29786
----------------------------------------------------------------------
[PARENT] Waiting for all child processes to reap exit status...
[REAPED] Child PID 29806  terminated normally with exit code 20
[REAPED] Child PID 29805  terminated normally with exit code 10
[REAPED] Child PID 29807  terminated normally with exit code 30
[REAPED] Child PID 29808  terminated normally with exit code 40
========================================================
All 4 child processes created, executed, and reaped successfully!
```

## Part B2: Parent waits for child with wait()

**Source:** [`parent_wait_child.c`](parent_wait_child.c)

```bash
gcc parent_wait_child.c -o parent_wait_child
./parent_wait_child
```

```
$ ./parent_wait_child
========================================================
   OS Lab 5 (b2): Parent Waiting for Child Completion
   Student: Aradhya Jain | Scholar No: 24112011111
========================================================
[1. PARENT] Starting execution. Parent PID: 30322
[1. PARENT] Invoking fork() to create a dedicated worker child...

[PARENT] Child created with PID: 30323
[PARENT] Entering wait(&status) - blocking until child completes...

  [CHILD] Hello from child process!
  [CHILD] My PID       : 30323
  [CHILD] My Parent PID: 30322
  [CHILD] Performing intensive task (simulated with 2s sleep)...
  [CHILD] Working... elapsed 1 second(s)
  [CHILD] Working... elapsed 2 second(s)
  [CHILD] Task completed successfully. Exiting with exit code 42.

[PARENT] Resumed after wait()!
[PARENT] Child PID returned by wait(): 30323
[PARENT] Normal termination detected (WIFEXITED == true).
[PARENT] Child exited with return status code: 42
[PARENT] Status matches expected child verification code (42).
========================================================
Parent-child lifecycle synchronization concluded cleanly.
```
