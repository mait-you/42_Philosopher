/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:47:55 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 12:25:07 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	init_forks(t_table *table)
{
	int	i;

	i = 0;
	memset(table->forks, 0, table->num_of_philos * sizeof(t_mtx));
	while (i < table->num_of_philos)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

static int	init_philos(t_table *table)
{
	int	i;

	i = 0;
	memset(table->philos, 0, table->num_of_philos * sizeof(t_philo));
	while (i < table->num_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].table = table;
		if (pthread_mutex_init(&table->philos[i].meal_lock_mutex, NULL) != 0)
			return (ERROR);
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[\
			(i + 1) % table->num_of_philos];
		i++;
	}
	return (SUCCESS);
}

static int	init_mutex(t_table *table)
{
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (error_cleanup(\
			table, "pthread_mutex_init", "table->print_mutex",
				"mutex init failed"));
	table->print_mutex_initialized = true;
	if (pthread_mutex_init(&table->simulation_mutex, NULL) != 0)
		return (error_cleanup(\
			table, "pthread_mutex_init", "table->simulation_mutex",
				"mutex init failed"));
	table->simulation_mutex_initialized = true;
	return (SUCCESS);
}

static int	get_args(t_table *table, int ac, char **av)
{
	time_t	cycle_time;

	table->eat_count = -1;
	table->num_of_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	if (ac == 6)
		table->eat_count = get_arg_as_num(av[5]);
	if (table->eat_count == 0)
		return (ERROR);
	if (table->time_to_eat == table->time_to_sleep)
	{
		cycle_time = table->time_to_eat + table->time_to_sleep;
		if (table->time_to_die - cycle_time > 10)
			table->time_to_thinking = 300;
		else
			table->time_to_thinking = 0;
	}
	fprintf(stderr, "table->time_to_thinking[%ld]\n", table->time_to_thinking);
	return (SUCCESS);
}


int	init_table(t_table *table, int ac, char **av)
{
	memset(table, 0, sizeof(t_table));
	if (get_args(table, ac, av) == ERROR)
		return (ERROR);
	if (init_mutex(table) == ERROR)
		return (ERROR);
	table->forks = malloc(table->num_of_philos * sizeof(t_mtx));
	if (!table->forks)
		return (error_cleanup(\
			table, "malloc()", "table->forks", MALLOC_ERROR));
	table->philos = malloc(table->num_of_philos * sizeof(t_philo));
	if (!table->philos)
		return (error_cleanup(\
			table, "malloc()", "table->philos", MALLOC_ERROR));
	if (init_forks(table) == ERROR)
		return (error_cleanup(\
			table, "pthread_mutex_init", "table->forks[i]",
				"mutex init failed"));
	if (init_philos(table) == ERROR)
		return (error_cleanup(\
			table, "pthread_mutex_init", "table->philos[i].meal_lock_mutex",
				"mutex init failed"));
	return (SUCCESS);
}
