/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/30 17:44:52 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"


long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(t_philo *philo, long time)
{
	long	start;
	long	elapsed;

	start = get_time_ms();
	while (!check_simulation_done(philo))
	{
		elapsed = get_time_ms() - start;
		if (elapsed >= time)
			break ;
		usleep(500);
	}
}

void	print_status(t_philo *philo, t_state status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->print_lock);
	if (check_simulation_done(philo) && status != DIED)
	{
		pthread_mutex_unlock(&philo->table->print_lock);
		return ;
	}
	timestamp = get_time_ms() - philo->table->simulation_start;
	if (status == TAKE_FORK)
		printf("%ld %d has taken a fork\n", timestamp, philo->id);
	else if (status == EATING)
		printf("%ld %d is eating\n", timestamp, philo->id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", timestamp, philo->id);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", timestamp, philo->id);
	else if (status == DIED)
	{
		printf("%ld %d died\n", timestamp, philo->id);
		pthread_mutex_lock(&philo->table->simulation_mutex);
		philo->table->simulation_done = 1;
		pthread_mutex_unlock(&philo->table->simulation_mutex);
	}
	pthread_mutex_unlock(&philo->table->print_lock);
}

int	check_simulation_done(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(&philo->table->simulation_mutex);
	done = philo->table->simulation_done;
	pthread_mutex_unlock(&philo->table->simulation_mutex);
	return (done);
}
