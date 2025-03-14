# 42_Philosopher


## What is a Thread?
A thread is a sequence of instructions within a process, managed by the operating system's kernel. Each thread has its own ID, stack, and registers but shares the same memory space with other threads in the process. Threads are faster to create and switch between than processes and can easily communicate through shared memory, though this can lead to synchronization issues. Unlike processes, threads do not follow a strict parent-child hierarchy.

