/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/17 08:57:09 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	check_death(t_table *table)
{
	int		i;
	long	time_lived;
	long	current_time;

	i = 0;
	current_time = get_time_ms();
	while (i < table->num_of_philos)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock_mutex);
		time_lived = current_time - table->philos[i].last_meal_time;
		if (time_lived > table->time_to_die)
		{
			pthread_mutex_unlock(&table->philos[i].meal_lock_mutex);
			print_status(&table->philos[i], DIED);
			return (ERROR);
		}
		pthread_mutex_unlock(&table->philos[i].meal_lock_mutex);
		i++;
	}
	return (SUCCESS);
}

static int	check_all_eat(t_table *table)
{
	int	i;
	int	finished_eating;

	if (table->eat_count <= 0)
		return (SUCCESS);
	i = 0;
	finished_eating = 0;
	while (i < table->num_of_philos)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock_mutex);
		if (table->philos[i].num_times_to_eat >= table->eat_count)
			finished_eating++;
		pthread_mutex_unlock(&table->philos[i].meal_lock_mutex);
		i++;
	}
	if (finished_eating == table->num_of_philos)
	{
		set_simulation_done(table);
		return (ERROR);
	}
	return (SUCCESS);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (!check_simulation_done(table->philos))
	{
		if (check_death(table))
			return (NULL);
		if (check_all_eat(table))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
