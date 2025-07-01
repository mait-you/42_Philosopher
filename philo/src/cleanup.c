/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:35 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/01 07:59:13 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	cleanup_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philos)
	{
		if (table->forks)
			pthread_mutex_destroy(&table->forks[i]);
		if (table->philos)
			pthread_mutex_destroy(&table->philos[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->simulation_mutex);
	if (table->philos)
	{
		free(table->philos);
		table->philos = NULL;
	}
	if (table->forks)
	{
		free(table->forks);
		table->forks = NULL;
	}
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
	cleanup_table(table);
	return (ERROR);
}
