# Philosophers Project (Bonus Part)

## Overview
The Philosophers project is a classic problem in computer science that simulates concurrent programming challenges. The bonus part of this project implements the dining philosophers problem using processes and semaphores, rather than threads and mutexes used in the mandatory part.

## Bonus Implementation vs Mandatory
| Aspect | Mandatory Part | Bonus Part |
|--------|---------------|------------|
| Philosopher representation | Thread | Process |
| Synchronization primitive | Mutex | Semaphore |
| Fork representation | One mutex per fork | Semaphore with value = number of philosophers |
| Fork placement | Between philosophers | In the middle of the table |

## Key Concepts

### Processes
A process is an instance of a program being executed with its own memory space, resources, and state. In this project:
- Each philosopher is implemented as a separate process using `fork()`
- Processes have isolated memory spaces, unlike threads
- Communication between processes requires Inter-Process Communication (IPC) mechanisms like semaphores
- The main process controls and monitors the simulation

### Semaphores
Semaphores are synchronization primitives that can be used to control access to shared resources across multiple processes:
- They work like counters that can be incremented (`sem_post`) or decremented (`sem_wait`)
- When a semaphore's value is 0, any process calling `sem_wait` will block until another process calls `sem_post`
- In this project, semaphores are used to:
  - Represent available forks (initialized to the number of philosophers)
  - Control access to printing (initialized to 1)
  - Protect access to meal timestamps (initialized to 1)

## New Functions Used

### Process Management
- `fork()` - Creates a new process (returns child PID to parent, 0 to child)
- `waitpid()` - Waits for a specific process to terminate
- `kill()` - Sends a signal to a process (used with SIGTERM to end philosopher processes)
- `exit()` - Terminates the current process with specified status

### Semaphore Management
- `sem_open()` - Creates/opens a named semaphore
- `sem_close()` - Closes a semaphore
- `sem_post()` - Increments a semaphore (releases a resource)
- `sem_wait()` - Decrements a semaphore (acquires a resource, blocks if not available)
- `sem_unlink()` - Removes a named semaphore from the system

### Thread Management (for monitoring)
- `pthread_create()` - Creates a monitoring thread within each philosopher process
- `pthread_detach()` - Detaches a thread so its resources are freed when it terminates
- `pthread_join()` - Waits for thread termination

## Project Structure

### Files Organization
- `main_bonus.c` - Entry point, manages processes
- `init_bonus.c` - Initializes table, philosophers, semaphores
- `philosophers_bonus.c` - Contains the philosopher routine
- `monitor_bonus.c` - Contains monitoring thread for death detection
- `parsing_bonus.c` - Argument parsing and validation
- `cleanup_bonus.c` - Resource cleanup functions
- `utils_bonus.c` - Utility functions (time, printing, etc.)

### Key Data Structures
1. **t_table** - Represents the dining table with:
   - Simulation parameters (number of philosophers, timing constraints)
   - Semaphores for synchronization
   - Array of philosophers

2. **t_philo** - Represents a philosopher with:
   - ID and process ID
   - Last meal timestamp
   - Reference to table
   - Monitor thread

3. **e_state** - Enum representing philosopher states:
   - TAKE_FORK, EATING, SLEEPING, THINKING, DIED

## Program Flow

1. Parse and validate command line arguments
2. Initialize table, philosophers, and semaphores
3. Create processes for each philosopher
4. In each philosopher process:
   - Create a monitoring thread to check for starvation
   - Execute philosopher routine (eat, sleep, think)
5. Parent process waits for all philosopher processes to complete
6. Clean up resources (close and unlink semaphores, free memory)

## Semaphore Usage

### Forks Semaphore
- Initialized to the number of philosophers
- Each philosopher decrements it twice to take two forks
- After eating, each philosopher increments it twice to return the forks

### Print Lock Semaphore
- Ensures atomic printing of status messages
- Initialized to 1 (only one process can print at a time)

### Meal Lock Semaphore
- Protects access to last meal timestamp
- Initialized to 1 (only one thread can access this data at a time)

## Tips for Implementation

1. **Managing Deadlocks**: Ensure philosophers don't all grab one fork simultaneously
   - Even-numbered philosophers can wait briefly before starting

2. **Death Detection**: Each philosopher has a monitoring thread that checks:
   - Time since last meal exceeds time_to_die
   - When this happens, the simulation ends

3. **Named Semaphores**: Always unlink semaphores at the start of the program and close them properly at the end to avoid resource leaks

4. **Resource Cleanup**: Be diligent about cleaning up all resources:
   - Close and unlink semaphores
   - Kill child processes
   - Free allocated memory

5. **Error Handling**: Check return values of system calls like fork(), sem_open()

## Common Challenges

1. **Race Conditions**: Be careful with timing, especially when checking for philosopher deaths

2. **Semaphore Synchronization**: Ensure proper ordering of sem_wait() and sem_post() calls

3. **Process Management**: Remember to handle all child processes properly to avoid zombies

4. **Resource Leaks**: Named semaphores and processes need to be properly cleaned up

## Testing Your Implementation

Test your program with different parameters, particularly edge cases:
- Single philosopher
- Very short time_to_die
- Very long time_to_eat or time_to_sleep
- Optional number_of_times_each_philosopher_must_eat parameter

## Additional Notes

- Make sure timestamps are accurate by using `gettimeofday()`
- Use `usleep()` for precise timing but implement "smart sleep" to avoid drift
- Remember that shared data between processes requires explicit synchronization using semaphores
- Death detection should be quick (< 10ms from actual death)
