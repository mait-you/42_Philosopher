/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:31:54 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/25 09:34:44 by mait-you         ###   ########.fr       */
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

# ifndef SUCCESS
#  define SUCCESS 0
# endif
# ifndef ERROR
#  define ERROR 1
# endif
# ifndef MAX_SLEEP_CHUNK
#  define MAX_SLEEP_CHUNK 250
# endif
# ifndef MIN_SLEEP_CHUNK
#  define MIN_SLEEP_CHUNK 10
# endif
# ifndef MAX_PHILOS
#  define MAX_PHILOS 200
# endif

# ifndef COLORS
#  define COLORS 0
# endif

# if COLORS == 1
#  define RED     "\e[1;31m"
#  define GREEN   "\e[1;32m"
#  define YELLOW  "\e[1;33m"
#  define CYAN    "\e[1;36m"
#  define GRAYL   "\e[90m"
#  define RESET   "\e[0m"
# else
#  define RED     ""
#  define GREEN   ""
#  define YELLOW  ""
#  define CYAN    ""
#  define GRAYL   ""
#  define RESET   ""
# endif

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
	time_t			sleep_chunk;
	int				eat_count;
	bool			simulation_done;
	time_t			time_simulation_start;
	t_mtx			print_mutex;
	t_mtx			simulation_mutex;
	t_mtx			*forks;
	t_philo			*philos;
	pthread_t		monitor_thread;
};

void	print_status(t_philo *philo, t_state status);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
void	ms_sleep(t_philo *philo, time_t time);
int		init_table(t_table *table, int ac, char **av);
time_t	get_time_ms(void);
int		error_msg(char *msg_type, char *the_error, char *msg);
int		error_cleanup(\
	t_table *table, char *msg_type, char *the_error, char *msg);
int		parsing(int ac, char **av);
int		get_arg_as_num(char *str);
int		check_simulation_done(t_philo *philo);
void	set_simulation_done(t_table *table);
int		cleanup_table(t_table *table);
int		join_philos(t_table *table, int num_of_philos);

#endif
