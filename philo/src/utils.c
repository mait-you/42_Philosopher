/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:06 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/01 16:12:03 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(time_t time)
{
	time_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < time)
		usleep(100);
	return ;
}

void	smart_usleep_check_simulation(t_philo *philo, time_t time)
{
	time_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < time)
	{
		if (check_simulation_done(philo))
			break ;
		usleep(100);
	}
	return ;
}

void	print_status(t_philo *philo, t_state status)
{
	time_t	current_time;

	if (check_simulation_done(philo))
		return ;
	pthread_mutex_lock(&philo->shared->print_lock);
	pthread_mutex_lock(&philo->shared->stop_mutex);
	current_time = get_time_in_ms() - philo->shared->simulation_start;
	if (!philo->shared->simulation_done && status == TAKE_FORK)
		printf("%12ld %12u has taken a fork\n", current_time, philo->id);
	else if (!philo->shared->simulation_done && status == EATING)
		printf("%12ld %12u is eating\n", current_time, philo->id);
	else if (!philo->shared->simulation_done && status == SLEEPING)
		printf("%12ld %12u is sleeping\n", current_time, philo->id);
	else if (!philo->shared->simulation_done && status == THINKING)
		printf("%12ld %12u is thinking\n", current_time, philo->id);
	else if (!philo->shared->simulation_done && status == DIED)
		printf("%12ld %12u died\n", current_time, philo->id);
	pthread_mutex_unlock(&philo->shared->stop_mutex);
	pthread_mutex_unlock(&philo->shared->print_lock);
}

int	check_simulation_done(t_philo *philo)
{
	int	should_stop;

	pthread_mutex_lock(&philo->shared->stop_mutex);
	should_stop = philo->shared->simulation_done;
	pthread_mutex_unlock(&philo->shared->stop_mutex);
	return (should_stop);
}
