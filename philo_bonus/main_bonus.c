/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:55:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 15:57:05 by mait-you         ###   ########.fr       */
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

static int wait_for_processes(t_table *table)
{
    int status;
    pid_t finished_pid;
    
    // Wait for any child process to finish
    finished_pid = waitpid(-1, &status, 0);
    
    if (finished_pid > 0)
    {
        // If any process exits with ERROR (death), kill all others
        if (WEXITSTATUS(status) == ERROR)
        {
            kill_all_processes(table);
            // Wait for all remaining processes to be killed
            while (waitpid(-1, NULL, WNOHANG) > 0)
                ;
            return (ERROR);
        }
        
        // If a process finished successfully, check if all are done
        // This handles the case where all philosophers ate enough times
        kill_all_processes(table);
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ;
    }
    
    return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (parsing(ac, av) == ERROR)
		return (ERROR);
	if (init_table(&table, ac, av) == ERROR)
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
