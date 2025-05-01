/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:31:54 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/01 17:24:47 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <string.h>

/* ************************************************************************** */
/* 							Status Codes and Messages                         */
/* ************************************************************************** */

/* Max philosophers */
# define MAX_PHILO 200
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
# define ARGS_ERROR "./philo <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]"
# define MALLOC_ERROR "memory allocation failed"

/* ************************************************************************** */
/* 							Data Structures                                   */
/* ************************************************************************** */

/* Typedef for all elements */
typedef pthread_mutex_t		t_mtx;
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
	pthread_t		philo_thread;
	int				num_times_to_eat;
	time_t			last_meal;
	t_mtx			*left_fork;
	t_mtx			*right_fork;
	t_mtx			meal_lock;
	t_table			*table;
};

struct s_table
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count;
	int				simulation_done;
	time_t			simulation_start;
	t_mtx			print_lock;
	t_mtx			stop_mutex;
	t_mtx			shared_mutex;
	pthread_t		monitor_thread;
	t_mtx			*forks;
	t_philo			*philos;
};

/* ************************************************************************** */
/* 							Function Prototypes                               */
/* ************************************************************************** */

void	print_status(t_philo *philo, t_state status);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
void	smart_sleep(time_t time);
void	smart_usleep_check_simulation(t_philo *philo, time_t time);
int		init_table(t_table *table, int ac, char **av);
time_t	get_time_in_ms(void);
int		error_msg(char *msg_type, char *the_error, char *msg);
int		error_cleanup(\
	t_table *table, char *msg_type, char *the_error, char *msg);
int		parsing(int ac, char **av);
int		get_arg_as_num(const char *str);
int		check_simulation_done(t_philo *philo);
void	table_cleanup(t_table *table);

#endif