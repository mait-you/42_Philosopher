/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:49 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/14 14:00:30 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_status(t_philo *philo, t_state status)
{
	time_t	current_time;

	if (is_simulation_done(philo->program))
		return ;
	pthread_mutex_lock(&philo->program->write_lock);
	current_time = get_time_in_ms(philo->program)
		- philo->table->simulation_start;
	if (status == TAKE_FORK)
		printf("%ld %u has taken a fork\n", current_time, philo->id);
	else if (status == EATING)
		printf("%ld %u is eating\n", current_time, philo->id);
	else if (status == SLEEPING)
		printf("%ld %u is sleeping\n", current_time, philo->id);
	else if (status == THINKING)
		printf("%ld %u is thinking\n", current_time, philo->id);
	else if (status == DIED)
		printf("%ld %u died\n", current_time, philo->id);
	pthread_mutex_unlock(&philo->program->write_lock);
}

static void *thinking(t_philo *philo)
{
	print_status(philo, THINKING);
	return (NULL);
}

static void *sleeping(t_philo *philo)
{
	print_status(philo, SLEEPING);
	smart_usleep(philo->program, philo->table->time_to_sleep);
	return (NULL);
}

static void *eating(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_FORK);
	if (philo->table->nb_philos == 1)
	{
		while (!is_simulation_done(philo->program))
			smart_usleep(philo->program, philo->table->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return (NULL);
	}
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, TAKE_FORK);
	philo->eating = 1;
	print_status(philo, EATING);
	pthread_mutex_lock(&philo->program->meal_lock);
	philo->last_meal = get_time_in_ms(philo->program);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->program->meal_lock);
	smart_usleep(philo->program, philo->table->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (NULL);
}

void *philosopher_routine(void *philo_ptr)
{
	t_philo *philo;

	philo = (t_philo *)philo_ptr;
	pthread_mutex_lock(&philo->program->meal_lock);
	philo->table->simulation_start = get_time_in_ms(philo->program);
	philo->last_meal = philo->table->simulation_start;
	pthread_mutex_unlock(&philo->program->meal_lock);
	if (philo->id % 2 == 0)
		smart_usleep(philo->program, philo->table->time_to_eat / 2);
	while (!is_simulation_done(philo->program))
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}