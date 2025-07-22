/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/19 15:42:51 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

time_t	get_time_ms(void)
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
		usleep(philo->table->sleep_chunk);
}

void	print_status(t_philo *philo, t_state status)
{
	time_t	current_time;

	sem_wait(philo->table->print_sem);
	check_simulation_done(philo);
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
		sem_wait(philo->table->stop_sem);
	}
	sem_post(philo->table->print_sem);
}

void	check_simulation_done(t_philo *philo)
{
	sem_wait(philo->table->stop_sem);
	sem_post(philo->table->stop_sem);
}
