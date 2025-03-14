/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:49 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/14 20:38:46 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Print the status of a philosopher with timestamp
 * 
 * @param philo Philosopher
 * @param status Status to print
 */
void	print_status(t_philo *philo, t_state status)
{
	time_t	current_time;

	if (is_simulation_done(philo->program) && status != DIED)
		return ;
	pthread_mutex_lock(&philo->program->write_lock);
	current_time
		= get_time_in_ms(philo->program) - philo->table->simulation_start;
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

/**
 * Philosopher thinking phase
 * 
 * @param philo Philosopher
 * @return NULL
 */
static void	*thinking(t_philo *philo)
{
	print_status(philo, THINKING);
	usleep(500);
	return (NULL);
}

/**
 * Philosopher sleeping phase
 * 
 * @param philo Philosopher
 * @return NULL
 */
static void	*sleeping(t_philo *philo)
{
	print_status(philo, SLEEPING);
	smart_usleep(philo->program, philo->table->time_to_sleep);
	return (NULL);
}

/**
 * Handle the single philosopher case specially
 * 
 * @param philo Philosopher
 * @return NULL
 */
static void	*handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_FORK);
	smart_usleep(philo->program, philo->table->time_to_die);
	pthread_mutex_unlock(philo->right_fork);
	return (NULL);
}

/**
 * Philosopher eating phase
 * 
 * @param philo Philosopher
 * @return NULL
 */
static void *eating(t_philo *philo)
{
	if (philo->table->num_of_philos == 1)
		return (handle_single_philo(philo), NULL);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, TAKE_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, TAKE_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, TAKE_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, TAKE_FORK);
	}
	pthread_mutex_lock(&philo->program->meal_lock);
	philo->last_meal = get_time_in_ms(philo->program);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->program->meal_lock);
	print_status(philo, EATING);
	smart_usleep(philo->program, philo->table->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

/**
 * Main philosopher routine
 * 
 * @param philo_ptr Pointer to philosopher
 * @return NULL
 */
void	*philosopher_routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	pthread_mutex_lock(&philo->program->meal_lock);
	philo->last_meal = get_time_in_ms(philo->program);
	pthread_mutex_unlock(&philo->program->meal_lock);
	if (philo->id % 2 == 0)
		smart_usleep(philo->program, philo->table->time_to_eat / 2);
	while (!is_simulation_done(philo->program))
	{
		eating(philo);
		if (is_simulation_done(philo->program))
			break ;
		sleeping(philo);
		if (is_simulation_done(philo->program))
			break ;
		thinking(philo);
	}
	return (NULL);
}
