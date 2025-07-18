/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 18:43:16 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/18 15:05:47 by mait-you         ###   ########.fr       */
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
	philo->last_meal_time = get_time_ms();
	philo->num_times_to_eat++;
	sem_post(philo->meal_sem);
	print_status(philo, EATING);
	ms_sleep(philo, philo->table->time_to_eat);
	sem_post(philo->table->forks_sem);
	sem_post(philo->table->forks_sem);
}

void	philosopher_routine(t_philo *philo)
{
	if (pthread_create(&philo->monitor_thread, NULL, \
		monitor_routine, philo) != 0)
		exit(ERROR);
	pthread_detach(philo->monitor_thread);
	if (philo->id % 2 == 1)
		usleep(philo->table->sleep_chunk);
	while (true)
	{
		eat(philo);
		check_simulation_done(philo);
		print_status(philo, SLEEPING);
		ms_sleep(philo, philo->table->time_to_sleep);
		print_status(philo, THINKING);
		check_simulation_done(philo);
	}
	exit(SUCCESS);
}
