/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 17:52:29 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	set_simulation_done(t_philo *philo)
{
   sem_wait(philo->table->simulation_sem);
   philo->table->simulation_done = true;
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	time_t	time_lived;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->meal_sem);
		time_lived = get_time_ms() - philo->last_meal_time;
		if (time_lived > philo->table->time_to_die)
		{
			print_status(philo, DIED);
			sem_post(philo->meal_sem);
			set_simulation_done(philo);
			exit(1);
		}
		if (philo->table->eat_count > 0
			&& philo->num_times_to_eat >= philo->table->eat_count)
		{
			sem_post(philo->meal_sem);
			set_simulation_done(philo);
			exit(1);
		}
		(sem_post(philo->meal_sem), usleep(100));
	}
	return (NULL);
}

void kill_all_processes(t_table *table)
{
    int i;

    i = 0;
    while (i < table->num_of_philos)
    {
        if (table->philos[i].pid > 0)
            kill(table->philos[i].pid, SIGTERM);
        i++;
    }
}