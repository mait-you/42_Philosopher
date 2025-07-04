/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 17:52:29 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	time_t	time_lived;

	philo = (t_philo *)arg;
	while (true)
	{
		sem_wait(philo->meal_sem);
		time_lived = get_time_ms() - philo->last_meal_time;
		if (time_lived > philo->table->time_to_die)
		{
			sem_post(philo->meal_sem);
			print_status(philo, DIED);
			exit(SUCCESS);
		}
		if (philo->table->eat_count > 0
			&& philo->num_times_to_eat >= philo->table->eat_count)
		{
			sem_post(philo->meal_sem);
			set_simulation_done(philo);
			exit(SUCCESS);
		}
		sem_post(philo->meal_sem);
		usleep(1000);
	}
	return (NULL);
}

void kill_all_processes(t_table *table)
{
	int i;

	i = 0;
	while (i < table->num_of_philos)
	{
		if (table->philos[i].pid != -1)
			kill(table->philos[i].pid, SIGTERM);
		i++;
	}
}
