/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/01 08:05:40 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_usleep(long time)
{
	long	start;
	long	elapsed;

	start = get_time_ms();
	while (1)
	{
		elapsed = get_time_ms() - start;
		if (elapsed >= time)
			break ;
		if (time - elapsed > 10)
			usleep((time - elapsed) * 500);
		else
		{
			while (get_time_ms() - start < time)
				;
		}
	}
}

void	print_status(t_philo *philo, t_state status)
{
	long	timestamp;

	sem_wait(philo->table->print_sem);
	timestamp = get_time_ms() - philo->table->start_time;
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
		sem_post(philo->table->stop_sem);
	}
	sem_post(philo->table->print_sem);
}
