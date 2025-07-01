/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/01 14:06:28 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"


long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long time)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time)
		usleep(500);
}

void	print_status(t_philo *philo, t_state status)
{
	long	timestamp;

	sem_wait(philo->table->print_sem);
	timestamp = get_time_ms() - philo->table->start_time;
	if (status == TAKE_FORK)
		printf(GRAYL"%ld %d has taken a fork\n"RESET, timestamp, philo->id);
	else if (status == EATING)
		printf(GREEN"%ld %d is eating\n"RESET, timestamp, philo->id);
	else if (status == SLEEPING)
		printf(CYAN"%ld %d is sleeping\n"RESET, timestamp, philo->id);
	else if (status == THINKING)
		printf(YELLOW"%ld %d is thinking\n"RESET, timestamp, philo->id);
	else if (status == DIED)
		printf(RED"%ld %d died\n"RESET, timestamp, philo->id);
	if (status != DIED)
		sem_post(philo->table->print_sem);
}
