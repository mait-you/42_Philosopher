/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/01 08:11:24 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	eat(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	print_status(philo, EATING);
	sem_wait(philo->meal_sem);
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	sem_post(philo->meal_sem);
	precise_usleep(philo->table->time_to_eat);
	sem_post(philo->table->forks_sem);
	sem_post(philo->table->forks_sem);
}

static void	single_philo_routine(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	precise_usleep(philo->table->time_to_die);
	print_status(philo, DIED);
	sem_post(philo->table->stop_sem);
	exit(ERROR);
}

void	philosopher_routine(t_philo *philo)
{
	if (pthread_create(&philo->monitor_thread, NULL, monitor_routine, philo))
		exit(ERROR);
	pthread_detach(philo->monitor_thread);
	if (philo->table->nb_philos == 1)
		single_philo_routine(philo);
	while (1)
	{
		eat(philo);
		print_status(philo, SLEEPING);
		precise_usleep(philo->table->time_to_sleep);
		print_status(philo, THINKING);
	}
}
