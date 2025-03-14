/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:00:59 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/14 13:58:57 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int start_simulation(t_program *program)
{
	unsigned int	i;

	i = 0;
	while (i < program->table.nb_philos)
	{
		if (ft_pthread_create(program, &program->philos[i].philo_thread,
			philosopher_routine, &program->philos[i]) != 0)
			return (1);
		i++;
	}
	if (ft_pthread_create(program, &program->table.monitor_thread,
		monitor_routine, program) != 0)
		return (1);
	return (0);
}

int stop_simulation(t_program *program)
{
	unsigned int	i;
	i = 0;
	while (i < program->table.nb_philos)
	{
		if (ft_pthread_join(program, &program->philos[i].philo_thread) != 0)
				return (1);
		i++;
	}
	if (ft_pthread_join(program, &program->table.monitor_thread) != 0)
				return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_program	program;

	if (ac != 5 && ac != 6)
		return (error_msg("Usage", NULL, ARGS_ERROR), 1);
	if (!parsing(ac, av))
		return (1);
	if (!init_program(&program, ac, av))
		return (1);
	if (start_simulation(&program) != 0)
		return (1);
	stop_simulation(&program);
	free_resources(&program);
	return (0);
}