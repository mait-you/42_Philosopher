/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/01 17:40:31 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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
	pthread_mutex_lock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->simulation_mutex);
	current_time = get_time_in_ms() - philo->table->simulation_start;
	if (!philo->table->simulation_done && status == TAKE_FORK)
		printf("%ld %u has taken a fork\n", current_time, philo->id);
	else if (!philo->table->simulation_done && status == EATING)
		printf("%ld %u is eating\n", current_time, philo->id);
	else if (!philo->table->simulation_done && status == SLEEPING)
		printf("%ld %u is sleeping\n", current_time, philo->id);
	else if (!philo->table->simulation_done && status == THINKING)
		printf("%ld %u is thinking\n", current_time, philo->id);
	else if (!philo->table->simulation_done && status == DIED)
		printf("%ld %u died\n", current_time, philo->id);
	pthread_mutex_unlock(&philo->table->simulation_mutex);
	pthread_mutex_unlock(&philo->table->print_lock);
}

int	check_simulation_done(t_philo *philo)
{
	int	should_stop;

	pthread_mutex_lock(&philo->table->simulation_mutex);
	should_stop = philo->table->simulation_done;
	pthread_mutex_unlock(&philo->table->simulation_mutex);
	return (should_stop);
}
