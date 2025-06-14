/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 15:43:16 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"


time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_usleep(t_philo *philo, time_t time)
{
	time_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < time)
	{
		if (check_simulation_done(philo))
			return ;
		usleep(100);
	}
}

void	print_status(t_philo *philo, t_state status)
{
	time_t	current_time;

	sem_wait(philo->table->print_lock);
	current_time = get_time_in_ms() - philo->table->simulation_start;
	
	if (status == TAKE_FORK)
		printf("%ld %d has taken a fork\n", current_time, philo->id);
	else if (status == EATING)
		printf("%ld %d is eating\n", current_time, philo->id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", current_time, philo->id);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", current_time, philo->id);
	else if (status == DIED)
	{
		printf("%ld %d died\n", current_time, philo->id);
		sem_post(philo->table->print_lock);
		return;
	}
	sem_post(philo->table->print_lock);
}

void	table_cleanup(t_table *table)
{
	close_semaphores(table);
	if (table->philos)
		free(table->philos);
}

int	check_simulation_done(t_philo *philo)
{
	int	should_stop;

	sem_wait(philo->table->simulation);
	should_stop = philo->table->simulation_done;
	sem_post(philo->table->simulation);
	return (should_stop);
}
