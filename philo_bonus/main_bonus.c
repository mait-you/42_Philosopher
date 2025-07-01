/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:55:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/01 08:10:48 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include_bonus/philo_bonus.h"

static int	start_simulation(t_table *table)
{
	int		i;
	pid_t	pid;

	table->start_time = get_time_ms();
	i = 0;
	while (i < table->nb_philos)
	{
		pid = fork();
		if (pid == 0)
		{
			table->philos[i].last_meal_time = table->start_time;
			philosopher_routine(&table->philos[i]);
		}
		else if (pid > 0)
			table->philos[i].pid = pid;
		else if (pid == -1)
		{
			kill_all_processes(table);
			return (error_msg("Fork failed", NULL, NULL));
		}
		i++;
	}
	return (SUCCESS);
}

static void	wait_for_processes(t_table *table)
{
	int	i;
	int	status;

	i = 0;
	while (i < table->nb_philos)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) == 1)
		{
			kill_all_processes(table);
			break ;
		}
		i++;
	}
}

int	main(int ac, char **av)
{
	t_table	table;

	if (parsing(ac, av) == ERROR)
		return (ERROR);
	if (init_table(&table, ac, av) == ERROR)
		return (ERROR);
	if (start_simulation(&table) == ERROR)
	{
		cleanup_table(&table);
		return (ERROR);
	}
	wait_for_processes(&table);
	cleanup_table(&table);
	return (SUCCESS);
}