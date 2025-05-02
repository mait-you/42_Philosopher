/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/01 18:15:53 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	reaper_of_souls(t_table *table, int *philos_done_eating)
{
	time_t	time_lived;
	int		i;

	i = 0;
	while (i < table->num_of_philos)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock);
		time_lived = get_time_in_ms() - table->philos[i].last_meal;
		if (time_lived > table->time_to_die)
		{
			print_status(&table->philos[i], DIED);
			pthread_mutex_lock(&table->simulation_mutex);
			table->simulation_done = 1;
			pthread_mutex_unlock(&table->simulation_mutex);
			pthread_mutex_unlock(&table->philos[i].meal_lock);
			return (1);
		}
		if (table->eat_count > 0 \
			&& table->philos[i].num_times_to_eat >= table->eat_count)
			(*philos_done_eating)++;
		pthread_mutex_unlock(&table->philos[i].meal_lock);
		i++;
	}
	return (0);
}

static int	should_stop(t_table *table, int philos_done_eating)
{
	if (table->eat_count > 0 \
			&& philos_done_eating == table->num_of_philos)
	{
		pthread_mutex_lock(&table->simulation_mutex);
		table->simulation_done = 1;
		pthread_mutex_unlock(&table->simulation_mutex);
		return (0);
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		philos_done_eating;

	table = (t_table *)arg;
	while (!check_simulation_done(table->philos))
	{
		philos_done_eating = 0;
		if (reaper_of_souls(table, &philos_done_eating))
			return (NULL);
		if (should_stop(table, philos_done_eating) == 0)
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
