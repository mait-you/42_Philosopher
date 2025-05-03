# Philosophers Project - Bonus part

## Basic Concepts

### Processes

A process is a running program with its own memory:

-   Each philosopher is a separate process made with `fork()`
-   Processes don't share memory like threads do
-   Processes need special ways to talk to each other
-   The main process watches over all philosopher processes

### Semaphores

Semaphores are like counters that help processes work together:

-   They can count up (`sem_post`) or count down (`sem_wait`)
-   When a semaphore is at 0, new processes must wait
-   In this project, semaphores are used for:
    -   Forks (count starts at number of philosophers)
    -   Printing (count starts at 1)
    -   Meal time tracking (count starts at 1)

### Named Semaphores

Named semaphores have special names so different processes can find them:

-   They have names like `/philo_forks`
-   Any process can use them if it knows the name
-   We must create, close, and remove them properly

### Process Communication

Since processes don't share memory, they need special ways to work together:

-   Semaphores to take turns using things
-   Signals to stop processes
-   Exit codes to tell the parent process what happened

## Special Functions Used

### Process Functions

-   `fork()` - Makes a new process
-   `waitpid()` - Waits for a process to end
-   `kill()` - Stops a process
-   `exit()` - Ends the current process

### Semaphore Functions

-   `sem_open()` - Makes or opens a named semaphore
-   `sem_close()` - Closes a semaphore
-   `sem_unlink()` - Removes a named semaphore
-   `sem_wait()` - Counts down a semaphore (waits if it's 0)
-   `sem_post()` - Counts up a semaphore (lets others continue)

### Thread Functions (inside each process)

-   `pthread_create()` - Makes a watcher thread
-   `pthread_join()` - Waits for a thread to finish

### Time Functions

-   `gettimeofday()` - Gets current time
-   `usleep()` - Pauses for a short time
