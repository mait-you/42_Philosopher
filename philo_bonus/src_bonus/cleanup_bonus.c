/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 18:43:29 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/25 08:36:25 by mait-you         ###   ########.fr       */
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
	if (table->finished_eating_sem)
		sem_close(table->finished_eating_sem);
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
	char	*id_str;

	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_STOP);
	sem_unlink(SEM_SIMUL);
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

int	error_cleanup(t_table *table, char *msg_type, char *the_error, char *msg)
{
	error_msg(msg_type, the_error, msg);
	cleanup_table(table);
	return (ERROR);
}

int	error_msg(char *msg_type, char *the_error, char *msg)
{
	ft_putstr_fd(RED, STDERR_FILENO);
	ft_putstr_fd("philo: ", STDERR_FILENO);
	if (msg_type)
	{
		ft_putstr_fd(YELLOW, STDERR_FILENO);
		ft_putstr_fd(msg_type, STDERR_FILENO);
	}
	if (the_error)
	{
		ft_putstr_fd(CYAN, STDERR_FILENO);
		ft_putstr_fd(": '", STDERR_FILENO);
		ft_putstr_fd(the_error, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
	}
	if (msg)
	{
		ft_putstr_fd(GRAYL, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(msg, STDERR_FILENO);
	}
	ft_putstr_fd(RESET, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (ERROR);
}
