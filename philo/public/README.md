# Philosophers Project - Mandatory part

## Basic Concepts

### Threads

Threads are like small workers that run at the same time:

-   Each philosopher is a thread
-   All threads can see and use the same data
-   We need special locks (mutexes) to stop problems
-   The main program starts philosopher threads and a watcher thread

### Mutexes (Locks)

Mutexes are like keys that control who can use something:

-   Only one thread can hold a mutex at a time
-   If a thread wants a locked mutex, it must wait
-   In this project, mutexes are used for:
    -   Forks (one mutex per fork)
    -   Protecting shared data (meal times, meal count)
    -   Making sure messages print clearly

### Data Race

A data race happens when two or more threads try to use the same data at the same time, and at least one is changing it:

-   This can happen if philosophers change shared data without locks
-   For example: two philosophers might try to change the meal count at the same time
-   We use mutexes to make sure only one philosopher can change data at a time
    > see `data_race.c` file

### Race Conditions

A race condition happens when the program acts differently based on which thread runs first:

-   Philosophers might try to take the same fork at the same time
-   We use mutexes to control the order and prevent problems
    > see `race_conditions.c` file

### Deadlocks

A deadlock is when all threads are stuck waiting for each other:

-   If all philosophers pick up their left fork first, no one can get their right fork
-   To fix this:
    -   Even-numbered philosophers take right fork first
    -   Odd-numbered philosophers take left fork first
    -   A simple delay at the beginning of `philosopher_routine`

## Special Functions Used

### Thread Functions

-   `pthread_create()` - Makes a new thread
-   `pthread_join()` - Waits for a thread to finish
-   `pthread_detach()` - Lets a thread clean up by itself

### Mutex Functions

-   `pthread_mutex_init()` - Creates a new mutex
-   `pthread_mutex_lock()` - Locks a mutex
-   `pthread_mutex_unlock()` - Unlocks a mutex

### Time Functions

-   `gettimeofday()` - Gets current time
-   `usleep()` - Pauses for a short time
