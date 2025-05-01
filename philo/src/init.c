/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:47:55 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/01 17:24:56 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	init_forks_and_philos(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->num_of_philos)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (ERROR);
	}
	i = -1;
	while (++i < table->num_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].num_times_to_eat = 0;
		table->philos[i].last_meal = get_time_in_ms();
		table->philos[i].table = table;
		if (pthread_mutex_init(&table->philos[i].meal_lock, NULL) != 0)
			return (ERROR);
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork
			= &table->forks[(i + 1) % table->num_of_philos];
	}
	return (SUCCESS);
}

static int	get_args(t_table *table, int ac, char **av)
{
	table->eat_count = -1;
	table->num_of_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	if (ac == 6)
		table->eat_count = get_arg_as_num(av[5]);
	if (table->eat_count == 0)
		return (ERROR);
	return (SUCCESS);
}

static int	init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&table->stop_mutex, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&table->shared_mutex, NULL) != 0)
		return (ERROR);
	return (SUCCESS);
}

int	init_table(t_table *table, int ac, char **av)
{
	memset(table, 0, sizeof(t_table));
	if (get_args(table, ac, av))
		return (ERROR);
	if (init_mutexes(table) == ERROR)
		return (ERROR);
	table->simulation_start = get_time_in_ms();
	table->forks
		= (t_mtx *)malloc(table->num_of_philos * sizeof(t_mtx));
	if (!table->forks)
		return (error_cleanup(table, NULL, NULL, NULL));
	table->philos
		= (t_philo *)malloc(table->num_of_philos * sizeof(t_philo));
	if (!table->philos)
		return (error_cleanup(table, NULL, NULL, NULL));
	if (init_forks_and_philos(table) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
