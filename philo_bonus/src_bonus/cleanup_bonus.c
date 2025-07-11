/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 18:43:29 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/11 08:41:47 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

static void	close_semaphores(t_table *table)
{
	int	i;

	if (table->forks_sem)
		sem_close(table->forks_sem);
	if (table->print_sem)
		sem_close(table->print_sem);
	if (table->stop_sem)
		sem_close(table->stop_sem);
	if (table->finished_sem)
		sem_close(table->finished_sem);
	if (table->philos)
	{
		i = 0;
		while (i < table->num_of_philos)
		{
			if (table->philos[i].meal_sem)
				sem_close(table->philos[i].meal_sem);
			i++;
		}
	}
}

void	unlink_semaphores(t_table *table)
{
	int		i;
	char	*sem_name;
	char	*id_str;

	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_STOP);
	sem_unlink(SEM_FINISHED);
	i = 0;
	while (i < table->num_of_philos)
	{
		id_str = ft_itoa(i + 1);
		if (id_str)
		{
			sem_name = ft_strjoin(SEM_MEAL, id_str);
			if (sem_name)
			{
				sem_unlink(sem_name);
				free(sem_name);
			}
		}
		i++;
	}
}

int	cleanup_table(t_table *table)
{
	close_semaphores(table);
	unlink_semaphores(table);
	if (table->philos)
	{
		free(table->philos);
		table->philos = NULL;
	}
	return (ERROR);
}

int	error_msg(char *msg_type, char *the_error, char *msg)
{
	printf("%sphilo: ", RED);
	if (msg_type)
		printf("%s%s", YELLOW, msg_type);
	if (the_error)
		printf("%s: '%s'", CYAN, the_error);
	if (msg)
		printf("%s: %s", GRAYL, msg);
	printf("%s\n", RESET);
	return (ERROR);
}

int	error_cleanup(t_table *table, char *msg_type, char *the_error, char *msg)
{
	error_msg(msg_type, the_error, msg);
	cleanup_table(table);
	return (ERROR);
}
