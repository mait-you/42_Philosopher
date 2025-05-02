/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/02 11:56:26 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, TAKE_FORK);
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, TAKE_FORK);
		pthread_mutex_lock(philo->right_fork);
	}
	print_status(philo, TAKE_FORK);
	pthread_mutex_lock(&philo->meal_lock);
	print_status(philo, EATING);
	philo->last_meal = get_time_in_ms();
	philo->num_times_to_eat++;
	pthread_mutex_unlock(&philo->meal_lock);
	smart_usleep_check_simulation(philo, philo->table->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return ;
}

static void	eat_for_one_philo(t_philo *philo)
{
	if (philo->table->num_of_philos == 1)
	{
		print_status(philo, TAKE_FORK);
		smart_usleep_check_simulation(philo, philo->table->time_to_die);
		print_status(philo, DIED);
	}
	return ;
}

void	*philosopher_routine(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	if (philo->table->num_of_philos == 1)
		return (eat_for_one_philo(philo), NULL);
	if (philo->id % 2 == 0)
		smart_usleep_check_simulation(philo, philo->table->time_to_eat);
	while (!check_simulation_done(philo))
	{
		eat(philo);
		if (check_simulation_done(philo))
			break ;
		print_status(philo, SLEEPING);
		smart_usleep_check_simulation(philo, philo->table->time_to_sleep);
		print_status(philo, THINKING);
	}
	return (NULL);
}
