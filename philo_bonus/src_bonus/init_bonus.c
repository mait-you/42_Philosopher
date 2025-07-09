/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:15 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 13:56:09 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static int	init_philosophers(t_table *table)
{
	int		i;
	char	*sem_name;
	char	*id_str;

	i = 0;
	memset(table->philos, 0, sizeof(t_philo) * table->num_of_philos);
	while (i < table->num_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].pid = -1;
		table->philos[i].table = table;
		id_str = ft_itoa(i + 1);
		if (!id_str)
			return (error_cleanup(table, NULL, NULL, "allocation failed"));
		sem_name = ft_strjoin(SEM_MEAL, id_str);
		if (!sem_name)
			return (error_cleanup(table, NULL, NULL, "allocation failed"));
		table->philos[i].meal_sem = sem_open(\
			sem_name, O_CREAT | O_EXCL, 0644, 1);
		free(sem_name);
		if (table->philos[i].meal_sem == SEM_FAILED)
			return (error_cleanup(table, NULL, NULL, "meal semaphore failed"));
		i++;
	}
	return (SUCCESS);
}

static int	init_semaphores(t_table *table)
{
	unlink_semaphores(table);
	table->forks_sem = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644,
			table->num_of_philos);
	if (table->forks_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create forks semaphore"));
	table->print_sem = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	if (table->print_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create print semaphore"));
	table->stop_sem = sem_open(SEM_STOP, O_CREAT | O_EXCL, 0644, 1);
	if (table->stop_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create stop semaphore"));
	table->finished_sem = sem_open(SEM_FINISHED, O_CREAT | O_EXCL, 0644, 1);
	if (table->finished_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create finished semaphore"));
	return (SUCCESS);
}

static int	get_args(t_table *table, int ac, char **av)
{
	time_t	cycle_time;

	table->eat_count = -1;
	table->num_of_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	if (ac == 6)
		table->eat_count = get_arg_as_num(av[5]);
	if (table->eat_count == 0)
		return (ERROR);
	if (table->time_to_eat == table->time_to_sleep)
	{
		cycle_time = table->time_to_eat + table->time_to_sleep;
		if (table->time_to_die - cycle_time > 10)
			table->time_to_thinking = 300;
		else
			table->time_to_thinking = 0;
	}
	table->sleep_chunk = table->num_of_philos * 2;
	if (table->sleep_chunk < 50)
		table->sleep_chunk = 50;
	else if (table->sleep_chunk > 500)
		table->sleep_chunk = 500;
	return (SUCCESS);
}

int	init_table(t_table *table, int ac, char **av)
{
	memset(table, 0, sizeof(t_table));
	if (get_args(table, ac, av) == ERROR)
		return (ERROR);
	if (init_semaphores(table) == ERROR)
		return (ERROR);
	table->philos = malloc(sizeof(t_philo) * table->num_of_philos);
	if (!table->philos)
		return (error_cleanup(table, NULL, NULL, MALLOC_ERROR));
	if (init_philosophers(table) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
