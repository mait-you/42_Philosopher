/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/01 14:11:29 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	check_meals_finished(t_philo *philo)
{
	sem_wait(philo->meal_sem);
	if (philo->table->eat_count > 0
		&& philo->meals_eaten >= philo->table->eat_count)
	{
		sem_post(philo->meal_sem);
		sem_wait(philo->table->finished_sem);
		philo->table->finished_count++;
		if (philo->table->finished_count == philo->table->num_of_philos)
			sem_post(philo->table->stop_sem);
		sem_post(philo->table->finished_sem);
		exit(SUCCESS);
	}
	sem_post(philo->meal_sem);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	long	time_since_meal;

	philo = (t_philo *)arg;
	while (true)
	{
		check_meals_finished(philo);
		sem_wait(philo->meal_sem);
		time_since_meal = get_time_ms() - philo->last_meal_time;
		if (time_since_meal >= philo->table->time_to_die)
		{
			sem_post(philo->meal_sem);
			print_status(philo, DIED);
			exit(ERROR);
		}
		sem_post(philo->meal_sem);
		usleep(1000);
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
			kill(table->philos[i].pid, SIGKILL);
		i++;
	}
}
