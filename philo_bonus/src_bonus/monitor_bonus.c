/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:48:28 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/24 11:07:53 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	check_meal_limit(t_philo *philo, bool *key)
{
	if (*key == false)
		return ;
	sem_wait(philo->meal_sem);
	if (philo->num_times_to_eat >= philo->table->eat_count)
	{
		sem_post(philo->table->finished_eating_sem);
		*key = false;
	}
	sem_post(philo->meal_sem);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	time_t	time_lived;
	bool	key;

	philo = (t_philo *)arg;
	key = true;
	while (true)
	{
		sem_wait(philo->meal_sem);
		time_lived = get_time_ms() - philo->last_meal_time;
		if (time_lived >= philo->table->time_to_die)
		{
			print_status(philo, DIED);
			sem_post(philo->meal_sem);
			exit(ERROR);
		}
		sem_post(philo->meal_sem);
		check_meal_limit(philo, &key);
		usleep(100);
	}
	return (NULL);
}

void	*check_all_eat(void *arg)
{
	t_table	*table;
	int		finished_eating;

	table = (t_table *)arg;
	finished_eating = 0;
	while (finished_eating < table->num_of_philos)
	{
		if (table->simulation_done)
			break ;
		sem_wait(table->finished_eating_sem);
		if (!table->simulation_done)
			finished_eating++;
	}
	if (finished_eating >= table->num_of_philos)
		kill_all_processes(table, 0);
	return (NULL);
}

int	kill_all_processes(t_table *table, pid_t ignore)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < table->num_of_philos)
	{
		pid = table->philos[i].pid;
		if (pid != -1 && pid != ignore)
		{
			kill(pid, SIGTERM);
			table->philos[i].pid = -1;
		}
		i++;
	}
	i = 0;
	while (++i < table->num_of_philos)
		waitpid(-1, NULL, 0);
	table->simulation_done = true;
	return (SUCCESS);
}
