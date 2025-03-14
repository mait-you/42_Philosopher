/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:42 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/12 16:06:45 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool init_philosopher(t_program *program)
{
	unsigned int	i;

	i = 0;
	while (i < program->table.nb_philos)
	{
		program->philos[i].id = i + 1;
		program->philos[i].left_fork = &program->forks[i].fork_lock;
		program->philos[i].right_fork = &program->forks
			[(i + 1) % program->table.nb_philos].fork_lock;
		program->philos[i].meals_eaten = 0;
		program->philos[i].eating = 0;
		program->philos[i].dead = 0;
		program->philos[i].num_times_to_eat = program->table.must_eat_count;
		program->philos[i].last_meal = get_time_in_ms(program);
		program->philos[i].table = &program->table;
		program->philos[i].program = program;
		i++;
	}
	return (true);
}

static bool init_forks(t_program *program, t_fork *forks)
{
	unsigned int	i;

	i = 0;
	while (i < program->table.nb_philos)
	{
		forks[i].id = i + 1;
		if (ft_pthread_mutex_init(program, &forks[i].fork_lock))
			return (false);
		i++;
	}
	return (true);
}

static bool	init_mutexes(t_program *program)
{
	if (ft_pthread_mutex_init(program, &program->dead_lock))
		return (false);
	if (ft_pthread_mutex_init(program, &program->meal_lock))
		return (false);
	if (ft_pthread_mutex_init(program, &program->write_lock))
		return (false);
	return (true);
}

static int	init_table(t_program *program, t_table *table, int ac, char **av)
{
	table->nb_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	if (ac == 6)
		table->must_eat_count = get_arg_as_num(av[5]);
	else if (ac == 5)
		table->must_eat_count = -1;
	table->program = program;
	table->simulation_done = false;
	return (0);
}

bool	init_program(t_program *program, int ac, char **av)
{
	memset(program, 0, sizeof(t_program));
	init_table(program, &program->table, ac, av);
	if (!init_mutexes(program))
		return (false);
	if (!init_forks(program, program->forks))
		return (false);
	if (!init_philosopher(program))
		return (false);
	program->table.simulation_start = get_time_in_ms(program);
	return (true);
}
