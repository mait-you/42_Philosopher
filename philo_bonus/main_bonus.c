/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:55:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 14:26:28 by mait-you         ###   ########.fr       */
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
		sem_wait(table->philos[i].meal_sem);
		table->philos[i].last_meal_time = get_time_ms();
		sem_post(table->philos[i].meal_sem);
		pid = fork();
		if (pid == 0)
			philosopher_routine(&table->philos[i]);
		else if (pid > 0)
			table->philos[i].pid = pid;
		else
			return (kill_all_processes(table),
				error_msg(NULL, NULL, "Fork failed"));
	}
	if (table->num_of_philos > 1
		&& pthread_create(&table->monitor_thread, NULL,
			monitor_routine, table) != 0)
			return (kill_all_processes(table), ERROR);
	return (SUCCESS);
}

static void	wait_for_processes(t_table *table)
{
	int			i;
	int			status;
	
	i = 0;
	while (i < table->num_of_philos)
	{
		if (waitpid(-1, &status, 0) > 0)
		{
			if ((WIFEXITED(status) || WIFSIGNALED(status))
				&& WEXITSTATUS(status) == ERROR)
			{
				table->simulation_done = 0;
				kill_all_processes(table);
				break ;
			}
		}
		i++;
	}
	if (table->num_of_philos > 1)
		pthread_join(table->monitor_thread, NULL);
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
		return (ERROR);
	}
	if (start_simulation(&table) == ERROR)
	{
		cleanup_table(&table);
		return (ERROR);
	}
	wait_for_processes(&table);
	cleanup_table(&table);
	return (SUCCESS);
}
