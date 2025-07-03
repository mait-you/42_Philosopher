/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:35 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/03 13:56:09 by mait-you         ###   ########.fr       */
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
	if (table->simulation_sem)
		sem_close(table->simulation_sem);
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

	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_STOP);
	sem_unlink(SEM_FINISHED);
	i = 0;
	while (i < table->num_of_philos)
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

void	cleanup_table(t_table *table)
{
	close_semaphores(table);
	unlink_semaphores(table);
	if (table->philos)
	{
		free(table->philos);
		table->philos = NULL;
	}
}

int	error_msg(char *msg_type, char *the_error, char *msg)
{
	if (msg_type && the_error && msg)
		printf("%sphilo: %s%s: %s'%s'%s: %s%s\n",
			RED, YELLOW, msg_type, CYAN, the_error, GRAYL, msg, RESET);
	else if (msg_type && the_error)
		printf("%sphilo: %s%s%s: '%s'%s\n",
			RED, YELLOW, msg_type, GRAYL, the_error, RESET);
	else if (msg_type && msg)
		printf("%sphilo: %s%s%s: %s%s\n",
			RED, YELLOW, msg_type, GRAYL, msg, RESET);
	else if (msg_type)
		printf("%sphilo: %s%s%s\n", RED, YELLOW, msg_type, RESET);
	else if (the_error && msg)
		printf("%sphilo: %s'%s'%s: %s%s\n",
			RED, CYAN, the_error, GRAYL, msg, RESET);
	else if (msg)
		printf("%sphilo: %s%s%s\n", RED, GRAYL, msg, RESET);
	return (ERROR);
}

int	error_cleanup(t_table *table, char *msg_type, char *the_error, char *msg)
{
	error_msg(msg_type, the_error, msg);
	cleanup_table(table);
	return (ERROR);
}
