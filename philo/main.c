/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:34:23 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/01 15:40:50 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	start_simulation(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philos)
	{
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
	return (0);
}

int	stop_simulation(t_table *table)
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
	return (0);
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
	return (0);
}
