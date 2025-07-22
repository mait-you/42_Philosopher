/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:55:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/21 09:30:49 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include_bonus/philo_bonus.h"

static int	start_simulation(t_table *table)
{
	int			i;
	pid_t		pid;

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
	if (table->finished_eating_sem && pthread_create(&table->checker, NULL, \
		check_all_eat, table) != 0)
		return (error_msg(NULL, NULL, "pthread_create failed"));
	return (SUCCESS);
}

static int	wait_for_processes(t_table *table)
{
	int			i;
	int			status;
	pid_t		pid;

	i = -1;
	pid = 1;
	while (pid != -1 && ++i < table->num_of_philos)
	{
		pid = waitpid(-1, &status, 0);
		if (pid > 0)
		{
			if (WIFEXITED(status) && WEXITSTATUS(status) == ERROR)
			{
				kill_all_processes(table, pid);
				break ;
			}
		}
	}
	i = -1;
	while (++i < table->num_of_philos)
		sem_post(table->finished_eating_sem);
	if (table->finished_eating_sem)
		pthread_join(table->checker, NULL);
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
