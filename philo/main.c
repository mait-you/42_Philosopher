/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:34:23 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/20 17:10:06 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

static int	start_simulation(t_table *table)
{
	int	i;

	i = 0;
	table->time_simulation_start = get_time_ms();
	while (i < table->num_of_philos)
	{
		table->philos[i].last_meal_time = get_time_ms();
		if (pthread_create(&table->philos[i].philo_thread, NULL,
				philosopher_routine, &table->philos[i]) != 0)
			return (join_philos(table, i), ERROR);
		i++;
	}
	if (table->num_of_philos > 1)
	{
		if (pthread_create(&table->monitor_thread, NULL,
				monitor_routine, table) != 0)
			return (join_philos(table, i), ERROR);
	}
	return (SUCCESS);
}

static int	stop_simulation(t_table *table)
{
	join_philos(table, table->num_of_philos);
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
	{
		if (table.eat_count == 0)
			return (SUCCESS);
		return (cleanup_table(&table));
	}
	if (start_simulation(&table) == ERROR)
		return (cleanup_table(&table));
	stop_simulation(&table);
	cleanup_table(&table);
	return (SUCCESS);
}
