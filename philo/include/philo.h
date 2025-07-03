/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:31:54 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 14:21:19 by mait-you         ###   ########.fr       */
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

# define SUCCESS 0
# define ERROR 1

# define RED     "\e[1;31m"
# define GREEN   "\e[1;32m"
# define YELLOW  "\e[1;35m"
# define CYAN    "\e[1;36m"
# define GRAYL   "\e[90m"
# define RESET   "\e[0m"

# define ARGS_ERROR "./philo <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]"
# define MALLOC_ERROR "Memory allocation failed"

typedef pthread_mutex_t		t_mtx;
typedef enum e_state		t_state;
typedef struct s_table		t_table;
typedef struct s_philo		t_philo;

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
	t_mtx			*left_fork;
	t_mtx			*right_fork;
	t_mtx			meal_lock_mutex;
	time_t			last_meal_time;
	pthread_t		philo_thread;
	t_table			*table;
};

struct s_table
{
	int				num_of_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				eat_count;
	bool			simulation_done;
	time_t			simulation_start;
	bool			print_mutex_initialized;
	t_mtx			print_mutex;
	bool			simulation_mutex_initialized;
	t_mtx			simulation_mutex;
	t_mtx			*forks;
	t_philo			*philos;
	pthread_t		monitor_thread;
};

void	print_status(t_philo *philo, t_state status);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
void	smart_sleep(t_philo *philo, long time);
int		init_table(t_table *table, int ac, char **av);
time_t	get_time_ms(void);
int		error_msg(char *msg_type, char *the_error, char *msg);
int		error_cleanup(\
	t_table *table, char *msg_type, char *the_error, char *msg);
int		parsing(int ac, char **av);
int		get_arg_as_num(const char *str);
int		check_simulation_done(t_philo *philo);
void	set_simulation_done(t_table *table);
void	cleanup_table(t_table *table);

#endif
