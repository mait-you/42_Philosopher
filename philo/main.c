/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:41:55 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/12 14:33:23 by mait-you         ###   ########.fr       */
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
	printf(YELLOW"[%u]\n"RESET, program.table.nb_philos);
	printf(YELLOW"[%u]\n"RESET, program.table.time_to_die);
	printf(YELLOW"[%u]\n"RESET, program.table.time_to_eat);
	printf(YELLOW"[%u]\n"RESET, program.table.time_to_sleep);
	printf(YELLOW"[%d]\n"RESET, program.table.must_eat_count);
	for (unsigned int i = 0; i < program.table.nb_philos; i++)
		printf(CYAN"philos id[%2u]\n"RESET, program.philos[i].id);
	for (unsigned int i = 0; i < program.table.nb_philos; i++)
		printf(CYAN"forks id [%2u]\n"RESET, program.forks[i].id);
	if (start_simulation(&program) != 0)
		return (1);
	stop_simulation(&program);
	free_resources(&program);
	return (0);
}
