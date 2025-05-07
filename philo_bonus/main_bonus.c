/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:55:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/07 16:39:45 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include_bonus/philo_bonus.h"

static int	start_simulation(t_table *table)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < table->num_of_philos)
	{
		pid = fork();
		if (pid == -1)
			return (ERROR);
		if (pid == 0)
			philosopher_routine(&table->philos[i]);
		else
			table->philos[i].pid = pid;
		i++;
	}
	return (SUCCESS);
}

static int	wait_for_processes(t_table *table)
{
	int	i;
	int	status;

	i = 0;
	while (i < table->num_of_philos)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) == ERROR)
			return (kill_all_processes(table), ERROR);
		i++;
	}
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_table	table;

	unlink_semaphores();
	if (parsing(ac, av) == ERROR)
		return (ERROR);
	if (init_table(&table, ac, av) != ERROR)
		return (cleanup_and_exit(&table), ERROR);
	if (start_simulation(&table) == ERROR)
	{
		kill_all_processes(&table);
		cleanup_and_exit(&table);
		return (ERROR);
	}
	wait_for_processes(&table);
	cleanup_and_exit(&table);
	return (SUCCESS);
}
