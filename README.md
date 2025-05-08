# 🍝 Dining Philosophers Project


```
███████╗██╗  ██╗██╗██╗      ██████╗ ███████╗ ██████╗ ███████╗██████╗███████╗██████╗ 
██╔══██║██║  ██║██║██║     ██╔═══██╗██╔════╝██╔═══██╗██╔══██║██╔═══╝██╔════╝██╔══██╗
███████║███████║██║██║     ██║   ██║███████╗██║   ██║███████║██████╗█████╗  ██████╔╝
██╔════╝██╔══██║██║██║     ██║   ██║╚════██║██║   ██║██╔════╝██╔═══╝██╔══╝  ██╔══██╗
██║     ██║  ██║██║███████╗ ██████╔╝███████║ ██████╔╝██║     ██║    ███████╗██║  ██║
╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝     ╚═╝    ╚══════╝╚═╝  ╚═╝                                                                               
```


This project is an implementation of the classic "Dining Philosophers" problem, a well-known synchronization and concurrency challenge in computer science.

## 📚 Problem Overview

The Dining Philosophers problem, formulated by Edsger Dijkstra in 1965, illustrates challenges in resource allocation and deadlock prevention:

- N philosophers sit at a round table with N forks
- Each philosopher needs two forks to eat (one on their left and one on their right)
- Philosophers alternate between thinking, eating, and sleeping
- Once a philosopher finishes eating, they put down the forks and start sleeping
- After sleeping, they start thinking again
- The simulation stops when a philosopher dies from starvation or when all philosophers have eaten a certain number of times

The challenge is to design a solution where no philosopher starves (dies) and to avoid deadlocks (where all philosophers hold one fork and wait indefinitely for another).

## 💡 Project Approach

I implemented two different versions of this problem:

1. **Mandatory Part (philo/)** - Using threads and mutexes
2. **Bonus Part (philo_bonus/)** - Using processes and semaphores

### Core Design Principles

- **Thread Safety**: Properly synchronizing access to shared resources
- **Deadlock Prevention**: Ensuring philosophers don't get stuck waiting for forks
- **Starvation Avoidance**: Ensuring no philosopher dies from lack of eating
- **Resource Efficiency**: Minimizing CPU usage with smart sleep techniques
- **Clean Memory Management**: Properly allocating and freeing resources

## 🧵 Mandatory Part Implementation (Threads & Mutexes)

### Data Structures

- `t_table`: Holds simulation parameters and shared resources
- `t_philo`: Represents each philosopher with their own attributes
- Mutexes for each fork and for other shared resources

### Key Components

1. **Initialization**
   - Parse and validate input arguments
   - Initialize mutexes, philosophers, and forks
   - Set up the simulation environment

2. **Philosopher Routine**
   - Each philosopher runs in its own thread
   - Implements the eat-sleep-think cycle
   - Uses an even/odd ID strategy to prevent deadlock (even IDs take right fork first, odd IDs take left fork first)

3. **Monitoring System**
   - A separate monitor thread watches all philosophers
   - Checks if any philosopher has died (hasn't eaten within time_to_die)
   - Checks if all philosophers have eaten enough times

4. **Synchronization Approach**
   - Each fork is protected by a mutex
   - Printing status is protected by a mutex to avoid garbled output
   - Meal times and meal counts are protected

5. **Deadlock Prevention**
   - Even-numbered philosophers take their right fork first, odd-numbered take their left fork first
   - This asymmetry breaks the circular wait condition that causes deadlock

## 🚀 Bonus Part Implementation (Processes & Semaphores)

### Key Differences from the Mandatory Part

- Each philosopher is a separate process (created with fork())
- Semaphores are used instead of mutexes for synchronization
- The monitoring is done within each philosopher process

### Semaphore Usage

- `SEM_FORKS`: Controls access to forks (initialized to the number of philosophers)
- `SEM_PRINT`: Controls access to standard output (initialized to 1)
- `SEM_MEAL`: Controls access to meal-related data (initialized to 1)

### Monitoring Approach

- Each philosopher process has a monitor thread
- This thread checks if the philosopher has starved
- If a philosopher dies, the process signals to others to terminate

## 🔄 Main Algorithm

1. Initialize simulation
2. Create philosopher threads/processes
3. Start monitor thread/process
4. Run the simulation until a philosopher dies or all have eaten enough
5. Join all threads/wait for all processes
6. Clean up resources

## ⚙️ Key Optimizations

1. **Smart Sleep**
   - Custom sleep function that checks simulation state periodically
   - Prevents wasting CPU cycles with busy waiting
   - Allows philosophers to respond quickly when simulation ends

2. **Time Management**
   - Using `gettimeofday()` for precise timestamp calculation
   - Tracking each philosopher's last meal time accurately

3. **Resource Handling**
   - Proper mutex/semaphore initialization and cleanup
   - Careful memory allocation and deallocation

## 🔍 Handling Edge Cases

1. **Single Philosopher**
   - Special case: with only one philosopher and one fork, they can never eat
   - Implementation handles this edge case separately

2. **Starvation Check**
   - Precise timing to detect when a philosopher has gone too long without eating

3. **Clean Termination**
   - Ensuring all threads/processes terminate cleanly
   - Proper resource cleanup

## 🏁 Simulation End Conditions

1. A philosopher dies (hasn't eaten for time_to_die milliseconds)
2. All philosophers have eaten at least number_of_times_each_philosopher_must_eat times (optional parameter)

## 📊 Performance Considerations

- Minimizing mutex/semaphore operations to improve performance
- Using efficient sleep methods to reduce CPU usage
- Careful thread/process synchronization to avoid race conditions

## 🛠️ Compilation and Usage

```bash
# Compile the mandatory part
cd philo
make

# Run the mandatory part
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

# Compile the bonus part
cd philo_bonus
make

# Run the bonus part
./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Arguments

1. `number_of_philosophers`: Number of philosophers and forks
2. `time_to_die`: Milliseconds until a philosopher dies if they haven't started eating
3. `time_to_eat`: Milliseconds it takes to eat
4. `time_to_sleep`: Milliseconds it takes to sleep
5. `[number_of_times_each_philosopher_must_eat]`: Optional - simulation stops when all philosophers have eaten this many times

## 📝 Conclusion

This project demonstrates solving a classic synchronization problem using both thread-based and process-based approaches. Through careful design and implementation, I was able to create a solution that prevents deadlocks and ensures fair resource allocation among all philosophers.

The key insight for solving the Dining Philosophers problem is breaking symmetry - either through the order of fork acquisition (as in my thread implementation) or through semaphore usage (in the process implementation).
