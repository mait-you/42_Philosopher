/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 14:23:31 by mait-you         ###   ########.fr       */
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
	timestamp = get_time_ms() - philo->table->simulation_start;
	if (status == TAKE_FORK)
		printf(GRAYL"%ld %d has taken a fork\n"RESET, timestamp, philo->id);
	else if (status == EATING)
		printf(GREEN"%ld %d is eating\n"RESET, timestamp, philo->id);
	else if (status == SLEEPING)
		printf(CYAN"%ld %d is sleeping\n"RESET, timestamp, philo->id);
	else if (status == THINKING)
		printf(YELLOW"%ld %d is thinking\n"RESET, timestamp, philo->id);
	else if (status == DIED)
	{
		printf(RED"%ld %d died\n"RESET, timestamp, philo->id);
		set_simulation_done(philo->table);
	}
	if (status != DIED)
		sem_post(philo->table->print_sem);
}

void	kill_all_processes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philos)
	{
		if (table->philos[i].pid > 0)
			kill(table->philos[i].pid, SIGKILL);
		i++;
	}
}

void	set_simulation_done(t_table *table)
{
	sem_wait(table->simulation_sem);
	table->simulation_done = true;	
	sem_post(table->simulation_sem);
}
