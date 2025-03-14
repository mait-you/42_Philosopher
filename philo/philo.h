/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:44 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/14 00:20:03 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# include <stdbool.h>
# include <errno.h>

# ifndef MAX_PHILO
#  define MAX_PHILO 200
# endif

/* ANSI color codes for terminal */
#define RED     "\e[1;31m"
#define YELLOW  "\e[1;35m"
#define CYAN    "\e[1;36m"
#define GRAYL   "\e[90m"
#define RESET   "\e[0m"

/* Errors messages */
# define ARGS_ERROR "./philo <number_of_philosophers> <time_to_die> \
<time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]"
# define PAR_MSG_1 "the argument must be unsigned integer between 0 and \
2147483647"
# define PAR_MSG_2 "<number_of_philosophers> is mora then 255"
# define PAR_MSG_3 "the argument is mora then 2147483647"

/* Typedef for all elements */
typedef pthread_mutex_t		t_mtx;
typedef enum e_state		t_state; 
typedef struct s_table		t_table;
typedef struct s_philo		t_philo;
typedef struct s_fork		t_fork;
typedef struct s_program	t_program;

/* Philosopher state */
enum e_state
{
	TAKE_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

/* Structure for each fork */
struct s_fork
{
	unsigned int	id;
	t_mtx			fork_lock;
};

/* Structure for each philosopher */
struct s_philo
{
	unsigned int	id;
	unsigned int	eating;
	unsigned int	dead;
	int				meals_eaten;
	int				num_times_to_eat;
	time_t			last_meal;
	pthread_t		philo_thread;
	t_mtx			*right_fork;
	t_mtx			*left_fork;
	t_table			*table;
	t_program		*program;
};

/* Structure for arguments */
struct s_table
{
	unsigned int	nb_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	must_eat_count;
	bool			simulation_done;
	time_t			simulation_start;
	pthread_t		monitor_thread;
	t_program		*program;
};

/* Structure for managing the program */
struct s_program
{
	t_table			table;
	t_philo			philos[MAX_PHILO];
	t_fork			forks[MAX_PHILO];
	t_mtx			dead_lock;
	t_mtx			meal_lock;
	t_mtx			write_lock;
};

/* Utils functions */
time_t		get_time_in_ms(t_program *program);
int			get_arg_as_num(char *str);
int			ft_pthread_create(t_program *program, pthread_t *thread, \
void *(*start_routine)(void *), void *arg);
int			ft_pthread_join(t_program *program, pthread_t *thread);
int			ft_pthread_mutex_init(t_program *program, t_mtx *mutex);
int			ft_pthread_mutex_lock(t_program *program, t_mtx *mutex);
int			ft_pthread_mutex_unlock(t_program *program, t_mtx *mutex);
int			ft_pthread_mutex_destroy(t_program *program, t_mtx *mutex);
int			smart_usleep(t_program *program, time_t ms);

/* Initialization functions */
bool		init_program(t_program *program, int ac, char **av);

/* Philosopher lifecycle functions */
void		*philosopher_routine(void *philo_ptr);
void		print_status(t_philo *philo, t_state status);

/* Monitoring functions */
void		*monitor_routine(void *program_ptr);
bool		is_simulation_done(t_program *program);
void		set_simulation_done(t_program *program);


/* Cleanup functions */
int 		error_cleanup(t_program *program, char *msg_type, char *the_error, char *msg);
int			error_msg(char *msg_type, char *the_error, char *msg);
int			free_resources(t_program *program);

/* parsing functions */
bool		parsing(int ac, char **av);

#endif