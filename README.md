# 🍝 Dining Philosophers Project

## Overview

The Dining Philosophers problem is a classic synchronization and concurrency challenge that illustrates the difficulties in resource allocation and process synchronization. This project implements a solution using threads and mutexes in C.

## Problem Description

- Several philosophers sit around a round table.
- In the middle is a big bowl of spaghetti.
- Each philosopher switches between eating, sleeping, and thinking.
- There is one fork between each two philosophers (so the number of forks = number of philosophers).
- To eat, a philosopher needs two forks — one from the left and one from the right.
- After eating:
  - The philosopher puts down both forks.
  - Then goes to sleep, and then thinks.
- The simulation ends when a philosopher dies from not eating in time.
- The goal is to make sure:
  - No philosopher starves.
  - All philosophers get to eat regularly.
  - Philosophers don’t communicate or know what others are doing.

## Project Structure

```
philo/
├── include/
│   └── philo.h        # Header files with definitions and prototypes
├── src/
│   ├── cleanup.c      # Resource cleanup functions
│   ├── init.c         # Initialization of data structures
│   ├── monitor.c      # Thread monitoring death conditions
│   ├── parsing.c      # Input argument parsing
│   ├── philosophers.c # Philosopher action implementations
│   └── utils.c        # Utility functions (time, etc.)
├── main.c             # Entry point
└── Makefile           # Build instructions
```

## Implementation Details

### Data Structures

1. **Philosopher Structure** (`t_philo`):
   - Unique ID (index)
   - Thread handle
   - Meal tracking information
   - Pointers to left and right fork mutexes
   - Access to shared data

2. **Table Structure** (`t_table`):
   - Simulation parameters (number of philosophers, timing values)
   - Monitoring thread handle
   - Array of forks (mutexes)
   - Array of philosophers
   - Synchronization mutexes for shared state

### Key Components

#### 1. Thread Management

Each philosopher runs in its own thread, executing a routine that cycles between:
- Taking forks
- Eating
- Sleeping
- Thinking

The main process spawns these threads and a monitoring thread.

#### 2. Mutex Usage

Mutexes (mutual exclusion locks) prevent race conditions:
- One mutex per fork prevents multiple philosophers from using the same fork
- A print lock ensures output doesn't get mixed
- A meal lock protects access to the last meal time
- A stop mutex protects the shared simulation state

#### 3. Deadlock Prevention

To prevent deadlocks where every philosopher holds one fork and waits:
- Even-numbered philosophers grab their right fork first, then left
- Odd-numbered philosophers grab their left fork first, then right
- A slight delay for even-numbered philosophers helps break symmetry

#### 4. Death Monitoring

A dedicated monitoring thread checks:
- If any philosopher has exceeded their time_to_die since their last meal
- If every philosopher has eaten the required number of times

## How It Works

1. **Initialization**:
   - Parse command line arguments
   - Initialize data structures, mutexes, and thread parameters
   - Set up forks and philosophers

2. **Simulation Start**:
   - Create philosopher threads
   - Create monitor thread
   - Start execution

3. **Philosopher Lifecycle**:
   - Take forks (in different order based on ID to prevent deadlock)
   - Eat (update last meal time)
   - Put down forks
   - Sleep
   - Think
   - Repeat until death or completion

4. **Monitoring**:
   - Monitor thread repeatedly checks all philosophers
   - If any philosopher hasn't eaten in time_to_die ms, they die
   - If all philosophers have eaten enough times, simulation stops

5. **Simulation End**:
   - Wait for all threads to complete
   - Clean up resources (free memory, destroy mutexes)

## Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Example:
```bash
./philo 5 800 200 200 10
```
This runs a simulation with:
- 5 philosophers
- Philosophers die if they don't eat within 800ms
- It takes 200ms to eat
- It takes 200ms to sleep
- Simulation stops when all philosophers eat at least 10 times

## Thread Synchronization Challenges

### Challenge 1: Race Conditions
- Multiple threads accessing/modifying shared data
- Solution: Use mutex locks around critical sections

### Challenge 2: Deadlocks
- Circular waiting can cause all philosophers to be stuck
- Solution: Break symmetry with different fork acquisition orders

### Challenge 3: Starvation
- Some philosophers might never get to eat
- Solution: Monitor thread ensures all get fair access to resources

## Implementation Highlights

1. **Time Management**:
   - Uses gettimeofday() for high precision timing
   - Smart sleep functions that don't block the entire process

2. **Resource Management**:
   - Proper mutex initialization and destruction
   - Clean exit with resource cleanup

3. **Edge Cases**:
   - Special handling for single philosopher case
   - Death detection with appropriate timing

## Key Learning Outcomes

This project demonstrates understanding of:
- Thread creation and management
- Race condition prevention with mutexes
- Deadlock avoidance techniques
- Resource allocation in concurrent systems
- Real-time monitoring in a multi-threaded environment
