/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/30 19:27:19 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	check_death(t_table *table)
{
	int		i;
	long	time_since_meal;
	long	current_time;

	i = 0;
	current_time = get_time_ms();
	while (i < table->num_of_philos)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock);
		time_since_meal = current_time - table->philos[i].last_meal;
		if (time_since_meal >= table->time_to_die)
		{
			pthread_mutex_unlock(&table->philos[i].meal_lock);
			print_status(&table->philos[i], DIED);
			return (ERROR);
		}
		pthread_mutex_unlock(&table->philos[i].meal_lock);
		i++;
	}
	return (SUCCESS);
}

static int	check_all_ate(t_table *table)
{
	int	i;
	int	finished_eating;

	if (table->eat_count <= 0)
		return (0);
	i = 0;
	finished_eating = 0;
	while (i < table->num_of_philos)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock);
		if (table->philos[i].num_times_to_eat >= table->eat_count)
			finished_eating++;
		pthread_mutex_unlock(&table->philos[i].meal_lock);
		i++;
	}
	if (finished_eating == table->num_of_philos)
	{
		pthread_mutex_lock(&table->simulation_mutex);
		table->simulation_done = 1;
		pthread_mutex_unlock(&table->simulation_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (!check_simulation_done(table->philos))
	{
		if (check_death(table))
			return (NULL);
		if (check_all_ate(table))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
