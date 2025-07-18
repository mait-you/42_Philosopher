/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/18 14:28:42 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ms_sleep(t_philo *philo, time_t time)
{
	time_t	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time)
	{
		if (check_simulation_done(philo))
			break ;
		usleep(philo->table->sleep_chunk);
	}
}

void	print_status(t_philo *philo, t_state status)
{
	time_t	current_time;

	pthread_mutex_lock(&philo->table->print_mutex);
	if (check_simulation_done(philo))
	{
		pthread_mutex_unlock(&philo->table->print_mutex);
		return ;
	}
	current_time = get_time_ms() - philo->table->simulation_start;
	if (status == TAKE_FORK)
		printf(GRAYL"%ld %d has taken a fork"RESET, current_time, philo->id);
	else if (status == EATING)
		printf(GREEN"%ld %d is eating"RESET, current_time, philo->id);
	else if (status == SLEEPING)
		printf(CYAN"%ld %d is sleeping"RESET, current_time, philo->id);
	else if (status == THINKING)
		printf(YELLOW"%ld %d is thinking"RESET, current_time, philo->id);
	else if (status == DIED)
	{
		printf(RED"%ld %d died"RESET, current_time, philo->id);
		set_simulation_done(philo->table);
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
}

int	check_simulation_done(t_philo *philo)
{
	int	done;

	pthread_mutex_lock(&philo->table->simulation_mutex);
	done = philo->table->simulation_done;
	pthread_mutex_unlock(&philo->table->simulation_mutex);
	return (done);
}

void	set_simulation_done(t_table *table)
{
	pthread_mutex_lock(&table->simulation_mutex);
	table->simulation_done = true;
	pthread_mutex_unlock(&table->simulation_mutex);
}
