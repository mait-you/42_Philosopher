/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:57:15 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/30 18:22:57 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	unlink_all_semaphores(t_table *table)
{
	int		i;
	char	*sem_name;

	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_STOP);
	i = 0;
	while (i < table->nb_philos)
	{
		sem_name = ft_strjoin(SEM_MEAL, ft_itoa(i + 1));
		if (sem_name)
		{
			sem_unlink(sem_name);
			free(sem_name);
		}
		i++;
	}
}

static int	init_semaphores(t_table *table)
{
	unlink_all_semaphores(table);
	table->forks_sem = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644,
			table->nb_philos);
	if (table->forks_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create forks semaphore"));
	table->print_sem = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	if (table->print_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create print semaphore"));
	table->stop_sem = sem_open(SEM_STOP, O_CREAT | O_EXCL, 0644, 0);
	if (table->stop_sem == SEM_FAILED)
		return (error_cleanup(\
			table, NULL, NULL, "Failed to create stop semaphore"));
	return (SUCCESS);
}

static int	init_philosophers(t_table *table)
{
	int		i;
	char	*sem_name;

	i = 0;
	while (i < table->nb_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_meal_time = 0;
		table->philos[i].pid = -1;
		table->philos[i].table = table;
		sem_name = ft_strjoin(SEM_MEAL, ft_itoa(i + 1));
		if (!sem_name)
			return (error_cleanup(\
				table, NULL, NULL, "String allocation failed"));
		table->philos[i].meal_sem = sem_open(sem_name, O_CREAT | O_EXCL,
				0644, 1);
		free(sem_name);
		if (table->philos[i].meal_sem == SEM_FAILED)
			return (error_cleanup(\
				table, NULL, NULL, "Failed to create meal semaphore"));
		i++;
	}
	return (SUCCESS);
}

int	init_table(t_table *table, int ac, char **av)
{
	memset(table, 0, sizeof(t_table));
	table->nb_philos = get_arg_as_num(av[1]);
	table->time_to_die = get_arg_as_num(av[2]);
	table->time_to_eat = get_arg_as_num(av[3]);
	table->time_to_sleep = get_arg_as_num(av[4]);
	table->must_eat_count = -1;
	if (ac == 6)
		table->must_eat_count = get_arg_as_num(av[5]);
	table->start_time = get_time_ms();
	if (init_semaphores(table) == ERROR)
		return (ERROR);
	table->philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!table->philos)
		return (error_cleanup(table, NULL, NULL, MALLOC_ERROR));
	if (init_philosophers(table) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
