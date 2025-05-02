/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/02 11:22:48 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	time_t	time_lived;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->table->meal_lock);
		time_lived = get_time_in_ms() - philo->last_meal;
		if (time_lived > philo->table->time_to_die)
		{
			print_status(philo, DIED);
			sem_post(philo->table->meal_lock);
			exit(ERROR);
		}
		if (philo->table->eat_count > 0
			&& philo->num_times_to_eat >= philo->table->eat_count)
		{
			sem_post(philo->table->meal_lock);
			exit(ERROR);
		}
		sem_post(philo->table->meal_lock);
		usleep(100);
	}
	return (NULL);
}

void	kill_all_processes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philos)
	{
		if (table->philos[i].pid > 0)
			kill(table->philos[i].pid, SIGTERM);
		i++;
	}
}
