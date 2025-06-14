/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:31:54 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 17:36:44 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <semaphore.h>

/* ************************************************************************** */
/* 							Status Codes and Messages                         */
/* ************************************************************************** */

/* Max philosophers */
# define SUCCESS 0
# define ERROR 1

/* ANSI color codes for terminal */
# define RED     "\e[1;31m"
# define GREEN   "\e[1;32m"
# define YELLOW  "\e[1;35m"
# define CYAN    "\e[1;36m"
# define GRAYL   "\e[90m"
# define RESET   "\e[0m"

/* Errors messages */
# define ARGS_ERROR "./philo_bonus <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]"
# define MALLOC_ERROR "memory allocation failed"

/* Semaphore names */
# define SEM_FORKS "/philo_forks"
# define SEM_PRINT "/philo_print"
# define SEM_MEAL "/philo_meal_"
# define SEM_DONE "/philo_done"

/* ************************************************************************** */
/* 							Data Structures                                   */
/* ************************************************************************** */

/* Typedef for all elements */
typedef enum e_state		t_state;
typedef struct s_table		t_table;
typedef struct s_philo		t_philo;

/* Philosopher state */
enum e_state
{
	TAKE_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
};

struct s_philo
{
	int				id;
	int				num_times_to_eat;
	pid_t			pid;
	time_t			last_meal;
	sem_t			*meal_lock;
	pthread_t		monitor_thread;
	t_table			*table;
};

struct s_table
{
	int				num_of_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				eat_count;
	int				simulation_done;
	time_t			simulation_start;
	sem_t			*forks;
	sem_t			*print_lock;
	sem_t			*simulation;
	t_philo			*philos;
};

/* ************************************************************************** */
/* 							Function Prototypes                               */
/* ************************************************************************** */

void	print_status(t_philo *philo, t_state status);
void	philosopher_routine(t_philo *philo);
void	*monitor_routine(void *arg);
void	smart_usleep(t_philo *philo, time_t time);
int		init_table(t_table *table, int ac, char **av);
time_t	get_time_in_ms(void);
int		error_msg(char *msg_type, char *the_error, char *msg);
int		error_cleanup(\
	t_table *table, char *msg_type, char *the_error, char *msg);
int		parsing(int ac, char **av);
int		get_arg_as_num(const char *str);
void	table_cleanup(t_table *table);
void	kill_all_processes(t_table *table);
void	close_semaphores(t_table *table);
void	unlink_semaphores(t_table *table);
void	cleanup_and_exit(t_table *table);
void	check_simulation_done(t_philo *philo);

size_t	ft_strlen(const char *s);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_itoa(int n);

#endif