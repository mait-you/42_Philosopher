/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 15:42:24 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	eat(t_philo *philo)
{
	sem_wait(philo->table->forks);
	print_status(philo, TAKE_FORK);
	sem_wait(philo->table->forks);
	print_status(philo, TAKE_FORK);
	sem_wait(philo->meal_lock);
	print_status(philo, EATING);
	philo->last_meal = get_time_in_ms();
	philo->num_times_to_eat++;
	sem_post(philo->meal_lock);
	smart_usleep(philo, philo->table->time_to_eat);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
}

static void	eat_for_one_philo(t_philo *philo)
{
	sem_wait(philo->table->forks);
	print_status(philo, TAKE_FORK);
	smart_usleep(philo, philo->table->time_to_die);
	print_status(philo, DIED);
	exit(SUCCESS);
}

void	philosopher_routine(t_philo *philo)
{
	if (pthread_create(&philo->monitor_thread, NULL, \
		monitor_routine, philo) != 0)
		exit(ERROR);
	pthread_detach(philo->monitor_thread);
	if (philo->table->num_of_philos == 1)
	{
		eat_for_one_philo(philo);
		return;
	}
	if (philo->id % 2 == 0)
		smart_usleep(philo, 10);
	while (1)
	{
		eat(philo);
		print_status(philo, SLEEPING);
		smart_usleep(philo, philo->table->time_to_sleep);
		print_status(philo, THINKING);
	}
	exit(SUCCESS);
}
