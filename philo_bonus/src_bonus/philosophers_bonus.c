/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:19 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 14:26:58 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static int	check_simulation_done(t_table *table)
{
	int	done;

	sem_wait(table->simulation_sem);
	done = table->simulation_done;
	sem_post(table->simulation_sem);
	return (done);
}

static int	check_eat_count(t_philo *philo)
{
	int	times_eaten;

	if (philo->table->eat_count <= 0)
		return (0);
	sem_wait(philo->meal_sem);
	times_eaten = philo->num_times_to_eat;
	sem_post(philo->meal_sem);
	return (times_eaten >= philo->table->eat_count);
}

static void	eat(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	print_status(philo, EATING);
	sem_wait(philo->meal_sem);
	philo->last_meal_time = get_time_ms();
	philo->num_times_to_eat++;
	sem_post(philo->meal_sem);
	smart_sleep(philo->table->time_to_eat);
	sem_post(philo->table->forks_sem);
	sem_post(philo->table->forks_sem);
}

static void	single_philo_routine(t_philo *philo)
{
	sem_wait(philo->table->forks_sem);
	print_status(philo, TAKE_FORK);
	smart_sleep(philo->table->time_to_die);
	print_status(philo, DIED);
	exit(SUCCESS);
}

void	philosopher_routine(t_philo *philo)
{
	if (philo->table->num_of_philos == 1)
		single_philo_routine(philo);
	
	if (philo->id % 2 == 0)
		smart_sleep(1);
	while (!check_simulation_done(philo->table))
	{
		eat(philo);
		if (check_eat_count(philo))
			break ;
		if (check_simulation_done(philo->table))
			break ;
		print_status(philo, SLEEPING);
		smart_sleep(philo->table->time_to_sleep);
		if (check_simulation_done(philo->table))
			break ;
		print_status(philo, THINKING);
	}
	exit(SUCCESS);
}
