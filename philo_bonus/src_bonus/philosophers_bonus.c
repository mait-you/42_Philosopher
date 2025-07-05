/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 17:53:58 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	eat(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	sem_wait(philo->meal_sem);
	print_status(philo, EATING);
	philo->last_meal_time = get_time_ms();
	philo->num_times_to_eat++;
	sem_post(philo->meal_sem);
	ms_sleep(philo, philo->table->time_to_eat);
	sem_post(philo->table->forks_sem);
	sem_post(philo->table->forks_sem);
}

static void	eat_for_one_philo(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	ms_sleep(philo, philo->table->time_to_die);
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
		eat_for_one_philo(philo);
	if (philo->id % 2 == 0)
		usleep(200);
	while (true)
	{
		eat(philo);
		print_status(philo, SLEEPING);
		ms_sleep(philo, philo->table->time_to_sleep);
		print_status(philo, THINKING);
		usleep(philo->table->time_to_thinking);
	}
	exit(SUCCESS);
}
