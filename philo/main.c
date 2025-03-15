/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:00:59 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/15 15:15:06 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Starts the simulation by creating philosopher threads and a monitor thread
 * 
 * @param program The main program structure
 * @return 0 on success, 1 on failure
 */
int	start_simulation(t_program *program)
{
	unsigned int	i;

	program->table.simulation_start = get_time_in_ms(program);
	i = 0;
	while (i < program->table.num_of_philos)
	{
		program->philos[i].last_meal = program->table.simulation_start;
		if (ft_pthread_create(program, &program->philos[i].philo_thread,
				philosopher_routine, &program->philos[i]) != 0)
			return (0);
		i++;
	}
	if (ft_pthread_create(program, &program->table.monitor_thread,
			monitor_routine, program) != 0)
		return (0);
	return (1);
}

/**
 * Stops the simulation by joining all threads
 * 
 * @param program The main program structure
 * @return 0 on success, 1 on failure
 */
int	stop_simulation(t_program *program)
{
	unsigned int	i;

	i = 0;
	while (i < program->table.num_of_philos)
	{
		if (ft_pthread_join(program, &program->philos[i].philo_thread) != 0)
			return (1);
		i++;
	}
	if (ft_pthread_join(program, &program->table.monitor_thread) != 0)
		return (1);
	return (0);
}

/**
 * Main function of the program
 * 
 * @param ac Argument count
 * @param av Argument vector
 * @return 0 on success, 1 on failure
 */
int	main(int ac, char **av)
{
	t_program	program;

	if (ac != 5 && ac != 6)
		return (error_msg("Usage", NULL, ARGS_ERROR), 1);
	if (!parsing(ac, av))
		return (1);
	if (!init_program(&program, ac, av))
		return (1);
	if (!start_simulation(&program))
		return (1);
	stop_simulation(&program);
	free_resources(&program);
	return (0);
}
