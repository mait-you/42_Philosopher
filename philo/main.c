/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:34:23 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/30 17:26:14 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

static int	start_simulation(t_table *table)
{
	int	i;

	i = 0;
	table->simulation_start = get_time_in_ms();
	while (i < table->num_of_philos)
	{
		table->philos[i].last_meal = get_time_in_ms();
		if (pthread_create(&table->philos[i].philo_thread, NULL,
				philosopher_routine, &table->philos[i]) != 0)
			return (ERROR);
		i++;
	}
	if (table->num_of_philos > 1)
	{
		if (pthread_create(&table->monitor_thread, NULL,
				monitor_routine, table) != 0)
			return (ERROR);
	}
	return (SUCCESS);
}

static int	stop_simulation(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philos)
	{
		pthread_join(table->philos[i].philo_thread, NULL);
		i++;
	}
	if (table->num_of_philos > 1)
		pthread_join(table->monitor_thread, NULL);
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (parsing(ac, av) == ERROR)
		return (ERROR);
	if (init_table(&table, ac, av) == ERROR)
		return (ERROR);
	if (start_simulation(&table) == ERROR)
	{
		table_cleanup(&table);
		return (ERROR);
	}
	stop_simulation(&table);
	table_cleanup(&table);
	return (SUCCESS);
}
