/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:35 by mait-you          #+#    #+#             */
/*   Updated: 2025/06/13 15:36:22 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

void	unlink_semaphores(t_table *table)
{
	int		i;
	char	*sem_name;


	i = -1;
	while (++i < table->num_of_philos)
	{
		sem_name = ft_strjoin(SEM_MEAL, ft_itoa(table->philos[i].id));
		sem_unlink(sem_name);
		free(sem_name);
	}
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_DONE);
}

void	close_semaphores(t_table *table)
{
	if (table->forks)
		sem_close(table->forks);
	if (table->print_lock)
		sem_close(table->print_lock);
	if (table->philos->meal_lock)
		sem_close(table->philos->meal_lock);
}

void	cleanup_and_exit(t_table *table)
{
	close_semaphores(table);
	unlink_semaphores(table);
	if (table && table->philos)
		free(table->philos);
}

int	error_msg(char *msg_type, char *the_error, char *msg)
{
	if (msg_type && the_error && msg)
		printf("%sphilo: %s%s: %s'%s'%s: %s%s\n",
			RED, YELLOW, msg_type,
			CYAN, the_error,
			GRAYL, msg, RESET);
	else if (msg_type && the_error)
		printf("%sphilo: %s%s%s: '%s'%s\n",
			RED, YELLOW, msg_type,
			GRAYL, the_error, RESET);
	else if (msg_type && msg)
		printf("%sphilo: %s%s%s: %s%s\n",
			RED, YELLOW, msg_type,
			GRAYL, msg, RESET);
	else if (msg_type)
		printf("%sphilo: %s%s%s\n",
			RED, YELLOW, msg_type, RESET);
	else if (the_error && msg)
		printf("%sphilo: %s'%s'%s: %s%s\n",
			RED, CYAN, the_error,
			GRAYL, msg, RESET);
	else if (msg)
		printf("%sphilo: %s%s%s\n",
			RED, GRAYL, msg, RESET);
	return (ERROR);
}

int	error_cleanup(
	t_table *table, char *msg_type, char *the_error, char *msg
	)
{
	error_msg(msg_type, the_error, msg);
	table_cleanup(table);
	return (ERROR);
}
