/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:15 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/25 09:02:10 by mait-you         ###   ########.fr       */
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
			sem_name, O_CREAT | O_EXCL, S_IRWXU, 1);
		free(sem_name);
		if (table->philos[i].meal_sem == SEM_FAILED)
			return (error_cleanup(table, NULL, NULL, "meal semaphore failed"));
		i++;
	}
	return (SUCCESS);
}

static int	init_semaphores(t_table *table)
{
	table->forks_sem = sem_open(SEM_FORKS, O_CREAT | O_EXCL, S_IRWXU,
			table->num_of_philos);
	if (table->forks_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create forks semaphore"));
	table->print_sem = sem_open(SEM_PRINT, O_CREAT | O_EXCL, S_IRWXU, 1);
	if (table->print_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create print semaphore"));
	table->stop_sem = sem_open(SEM_STOP, O_CREAT | O_EXCL, S_IRWXU, 1);
	if (table->stop_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create stop semaphore"));
	table->simulation_sem = sem_open(SEM_SIMUL, O_CREAT | O_EXCL, S_IRWXU, 1);
	if (table->simulation_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create simulation semaphore"));
	if (table->eat_count <= 0)
		return (SUCCESS);
	table->finished_eating_sem = sem_open(\
		SEM_FINISHED, O_CREAT | O_EXCL, S_IRWXU, table->num_of_philos);
	if (table->finished_eating_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create finished semaphore"));
	return (SUCCESS);
}

static int	get_args(t_table *table, int ac, char **av)
{
	float	a;
	int		b;

	table->eat_count = -1;
	table->num_of_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	if (ac == 6)
		table->eat_count = get_arg_as_num(av[5]);
	if (table->eat_count == 0)
		return (ERROR);
	a = (float)(MAX_SLEEP_CHUNK - MIN_SLEEP_CHUNK) / MAX_PHILOS;
	b = MIN_SLEEP_CHUNK;
	table->sleep_chunk = (int)(a * table->num_of_philos + b);
	return (SUCCESS);
}

int	init_table(t_table *table, int ac, char **av)
{
	int	i;

	memset(table, 0, sizeof(t_table));
	if (get_args(table, ac, av) == ERROR)
		return (ERROR);
	unlink_semaphores(table);
	if (init_semaphores(table) == ERROR)
		return (ERROR);
	i = -1;
	while (++i < table->num_of_philos)
		sem_wait(table->finished_eating_sem);
	table->philos = malloc(sizeof(t_philo) * table->num_of_philos);
	if (!table->philos)
		return (error_cleanup(table, NULL, NULL, "memory allocation failed"));
	if (init_philosophers(table) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
