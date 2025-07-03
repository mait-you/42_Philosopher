/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 14:10:25 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static int	check_death(t_table *table)
{
	int		i;
	long	time_since_meal;
	long	current_time;

	i = 0;
	current_time = get_time_ms();
	while (i < table->num_of_philos)
	{
		sem_wait(table->philos[i].meal_sem);
		time_since_meal = current_time - table->philos[i].last_meal_time;
		if (time_since_meal >= table->time_to_die)
		{
			sem_post(table->philos[i].meal_sem);
			print_status(&table->philos[i], DIED);
			return (ERROR);
		}
		sem_post(table->philos[i].meal_sem);
		i++;
	}
	return (SUCCESS);
}

static int	check_all_ate(t_table *table)
{
	int	finished_eating;
	int	i;

	if (table->eat_count <= 0)
		return (SUCCESS);
	finished_eating = 0;
	i = 0;
	while (i < table->num_of_philos)
	{
		sem_wait(table->philos[i].meal_sem);
		if (table->philos[i].num_times_to_eat >= table->eat_count)
			finished_eating++;
		sem_post(table->philos[i].meal_sem);
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
	while (table->simulation_done)
	{
		if (check_all_ate(table))
			return (NULL);
		if (check_death(table))
			return (NULL);
		usleep(1000);
	}
	kill_all_processes(table);
	return (NULL);
}
