/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/24 20:36:09 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_FORK);
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, TAKE_FORK);
	pthread_mutex_lock(&philo->meal_lock_mutex);
	philo->last_meal_time = get_time_ms();
	philo->num_times_to_eat++;
	print_status(philo, EATING);
	pthread_mutex_unlock(&philo->meal_lock_mutex);
	ms_sleep(philo, philo->table->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	*single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_FORK);
	ms_sleep(philo, philo->table->time_to_die);
	print_status(philo, DIED);
	pthread_mutex_unlock(philo->right_fork);
	return (NULL);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->num_of_philos == 1)
		return (single_philo(philo));
	philo->last_meal_time = get_time_ms();
	if (philo->id % 2 == 0)
		ms_sleep(philo, 10);
	while (!check_simulation_done(philo))
	{
		eat(philo);
		if (check_simulation_done(philo))
			break ;
		print_status(philo, SLEEPING);
		ms_sleep(philo, philo->table->time_to_sleep);
		if (check_simulation_done(philo))
			break ;
		print_status(philo, THINKING);
	}
	return (NULL);
}
