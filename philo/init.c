/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/15 15:16:29 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Initialize each philosopher with their properties
 * 
 * @param program The main program structure
 * @param philos philosopher structure
 * @return true on success, false on failure
 */
static bool	init_philosopher(t_program *program, t_philo *philos)
{
	unsigned int	i;

	if (!program->philos)
		return (false);
	memset(philos, 0, sizeof(t_philo));
	i = 0;
	while (i < program->table.num_of_philos)
	{
		program->philos[i].id = i + 1;
		program->philos[i].left_fork = &program->forks[i].fork_lock;
		program->philos[i].right_fork
			= &program->forks[(i + 1) % program->table.num_of_philos].fork_lock;
		program->philos[i].table = &program->table;
		program->philos[i].program = program;
		i++;
	}
	return (true);
}

/**
 * Initialize forks with their properties and mutex locks
 * 
 * @param program The main program structure
 * @param forks Array of forks
 * @return true on success, false on failure
 */
static bool	init_forks(t_program *program, t_fork *forks)
{
	unsigned int	i;

	if (!forks)
		return (false);
	memset(forks, 0, sizeof(t_fork));
	i = 0;
	while (i < program->table.num_of_philos)
	{
		forks[i].id = i + 1;
		if (ft_pthread_mutex_init(program, &forks[i].fork_lock))
			return (false);
		i++;
	}
	return (true);
}

/**
 * Initialize all required mutexes
 * 
 * @param program The main program structure
 * @return true on success, false on failure
 */
static bool	init_mutexes(t_program *program)
{
	if (ft_pthread_mutex_init(program, &program->dead_lock))
		return (false);
	if (ft_pthread_mutex_init(program, &program->meal_lock))
		return (false);
	if (ft_pthread_mutex_init(program, &program->write_lock))
		return (false);
	return (true);
}

/**
 * Initialize the table with values from command line arguments
 * 
 * @param program The main program structure
 * @param table The table structure
 * @param ac Argument count
 * @param av Argument vector
 * @return 0 on success
 */
static bool	init_table(t_program *program, t_table *table, int ac, char **av)
{
	table->num_of_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	if (ac == 6)
		table->must_eat_count = get_arg_as_num(av[5]);
	else if (ac == 5)
		table->must_eat_count = -1;
	if (table->must_eat_count == 0)
		return (false);
	table->program = program;
	table->simulation_done = false;
	return (true);
}

/**
 * Initialize the whole program
 * 
 * @param program The main program structure
 * @param ac Argument count
 * @param av Argument vector
 * @return true on success, false on failure
 */
bool	init_program(t_program *program, int ac, char **av)
{
	memset(program, 0, sizeof(t_program));
	if (!init_table(program, &program->table, ac, av))
		return (false);
	if (!init_mutexes(program))
		return (false);
	program->philos
		= (t_philo *)malloc(program->table.num_of_philos * sizeof(t_philo));
	if (!program->philos)
		return (error_cleanup(program, NULL, NULL, MALLOC_ERROR), false);
	program->forks
		= (t_fork *)malloc(program->table.num_of_philos * sizeof(t_fork));
	if (!program->forks)
		return (error_cleanup(program, NULL, NULL, MALLOC_ERROR), false);
	if (!init_forks(program, program->forks))
		return (false);
	if (!init_philosopher(program, program->philos))
		return (false);
	return (true);
}
