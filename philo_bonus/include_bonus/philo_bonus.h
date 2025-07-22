/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:31:54 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/22 15:57:45 by mait-you         ###   ########.fr       */
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
# include <stdbool.h>
# include <string.h>
# include <semaphore.h>
# include <signal.h>

# ifndef SUCCESS
#  define SUCCESS 0
# endif
# ifndef ERROR
#  define ERROR 1
# endif
# ifndef MAX_SLEEP_CHUNK
#  define MAX_SLEEP_CHUNK 300
# endif
# ifndef MIN_SLEEP_CHUNK
#  define MIN_SLEEP_CHUNK 10
# endif
# ifndef MAX_PHILOS
#  define MAX_PHILOS 200
# endif

# ifndef SEM_FORKS
#  define SEM_FORKS "/philo_forks"
# endif
# ifndef SEM_PRINT
#  define SEM_PRINT "/philo_print"
# endif
# ifndef SEM_MEAL
#  define SEM_MEAL "/philo_meal_"
# endif
# ifndef SEM_STOP
#  define SEM_STOP "/philo_stop"
# endif
# ifndef SEM_FINISHED
#  define SEM_FINISHED "/philo_finished_eating"
# endif

# ifndef COLORS
#  define COLORS 90
# endif

# if COLORS == 1
#  define RED     "\e[1;31m"
#  define GREEN   "\e[1;32m"
#  define YELLOW  "\e[1;33m"
#  define CYAN    "\e[1;36m"
#  define GRAYL   "\e[90m"
#  define RESET   "\e[0m\n"
# else
#  define RED     ""
#  define GREEN   ""
#  define YELLOW  ""
#  define CYAN    ""
#  define GRAYL   ""
#  define RESET   "\n"
# endif

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
	pid_t			pid;
	int				num_times_to_eat;
	time_t			last_meal_time;
	sem_t			*meal_sem;
	pthread_t		monitor_thread;
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
	time_t			simulation_start;
	sem_t			*forks_sem;
	sem_t			*print_sem;
	sem_t			*stop_sem;
	sem_t			*finished_eating_sem;
	pthread_t		checker;
	t_philo			*philos;
};

int		parsing(int ac, char **av);
void	philosopher_routine(t_philo *philo);
int		init_table(t_table *table, int ac, char **av);
void	*monitor_routine(void *arg);
void	print_status(t_philo *philo, t_state status);
void	ms_sleep(t_philo *philo, time_t time);
time_t	get_time_ms(void);
char	*ft_itoa(int n);
char	*ft_strjoin(char *s1, char *s2);
int		cleanup_table(t_table *table);
int		kill_all_processes(t_table *table, pid_t ignore);
int		error_msg(char *msg_type, char *the_error, char *msg);
int		get_arg_as_num(char *str);
int		error_cleanup(\
	t_table *table, char *msg_type, char *the_error, char *msg);
void	unlink_semaphores(t_table *table);
void	check_simulation_done(t_philo *philo);
void	ft_putstr_fd(char *s, int fd);
void	*check_all_eat(void *arg);

#endif