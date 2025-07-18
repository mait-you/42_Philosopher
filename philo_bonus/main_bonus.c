/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:55:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/18 15:34:14 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include_bonus/philo_bonus.h"

static int	start_simulation(t_table *table)
{
	int		i;
	pid_t	pid;

	i = -1;
	table->simulation_start = get_time_ms();
	while (++i < table->num_of_philos)
	{
		table->philos[i].last_meal_time = get_time_ms();
		pid = fork();
		if (pid == 0)
			philosopher_routine(&table->philos[i]);
		else if (pid > 0)
			table->philos[i].pid = pid;
		else
			return (kill_all_processes(table, 0),
				error_msg(NULL, NULL, "Fork failed"));
	}
	return (SUCCESS);
}

static void	*check_all_eat(void *arg)
{
	t_table	*table;
	int		finished_eating;

	table = (t_table *)arg;
	finished_eating = 0;
	while (finished_eating <= table->num_of_philos)
	{
		sem_post(table->finished_eating_sem);
			finished_eating++;
	}
	kill_all_processes(table, 0);
	return (NULL);
}

static int	wait_for_processes(t_table *table)
{
	int			i;
	int			status;
	pid_t		pid;
	pthread_t	checker;

	i = -1;
	if (pthread_create(&checker, NULL, \
		check_all_eat, table) != 0)
		return (error_msg(NULL, NULL, "pthread_create failed"));
	while (++i < table->num_of_philos)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			return (error_msg(NULL, NULL, "Waitpid failed"),
				kill_all_processes(table, 0));
		if (pid > 0)
		{
			if (WIFEXITED(status) && WEXITSTATUS(status) == ERROR)
				return (kill_all_processes(table, pid));
		}
	}
	pthread_join(checker, NULL);
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (parsing(ac, av) == ERROR)
		return (ERROR);
	if (init_table(&table, ac, av) == ERROR)
	{
		if (table.eat_count == 0)
			return (SUCCESS);
		return (cleanup_table(&table));
	}
	if (start_simulation(&table) == ERROR)
		return (cleanup_table(&table));
	wait_for_processes(&table);
	cleanup_table(&table);
	return (SUCCESS);
}
