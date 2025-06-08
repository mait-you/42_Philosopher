/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enoki <enoki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:15 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/08 12:02:30 by enoki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static int	init_philosophers(t_table *table)
{
	int		i;
	char	*sem_name;

	i = 0;
	sem_name = NULL;
	while (i < table->num_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].num_times_to_eat = 0;
		table->philos[i].last_meal = get_time_in_ms();
		table->philos[i].table = table;
		table->philos[i].pid = -1;
		sem_name = ft_strjoin(SEM_MEAL, ft_itoa(table->philos[i].id));
		table->philos[i].meal_lock = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1);
		if (sem_name)
		{
			free(sem_name);
			sem_name = NULL;
		}
		if (table->philos[i].meal_lock == SEM_FAILED)
			return (ERROR);
		i++;
		
	}
	return (SUCCESS);
}

static int	init_semaphores(t_table *table)
{
	table->forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644, \
		table->num_of_philos);
	if (table->forks == SEM_FAILED)
		return (ERROR);
	table->print_lock = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	if (table->print_lock == SEM_FAILED)
		return (ERROR);
	return (SUCCESS);
}

static int	get_args(t_table *table, int ac, char **av)
{
	table->eat_count = -1;
	table->num_of_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	if (ac == 6)
		table->eat_count = get_arg_as_num(av[5]);
	if (table->eat_count == 0)
		return (ERROR);
	return (SUCCESS);
}

int	init_table(t_table *table, int ac, char **av)
{
	memset(table, 0, sizeof(t_table));
	if (get_args(table, ac, av) == ERROR)
		return (ERROR);
	table->simulation_start = get_time_in_ms();
	table->simulation_done = 0;
	if (init_semaphores(table) == ERROR)
		return (error_msg("semaphore", NULL, "failed to initialize semaphores"));
	table->philos = (t_philo *)malloc(table->num_of_philos * sizeof(t_philo));
	if (!table->philos)
		return (error_cleanup(table, NULL, NULL, MALLOC_ERROR));
	unlink_semaphores(table);
	if (init_philosophers(table) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
