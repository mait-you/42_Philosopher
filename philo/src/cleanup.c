/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:35 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/10 09:58:15 by mait-you         ###   ########.fr       */
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
			pthread_mutex_destroy(&table->philos[i].meal_lock_mutex);
		i++;
	}
	if (table->print_mutex_initialized)
		pthread_mutex_destroy(&table->print_mutex);
	if (table->simulation_mutex_initialized)
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

int	join_philos(t_table *table, int num_of_philos)
{
	int	i;

	i = 0;
	while (i < num_of_philos)
	{
		pthread_join(table->philos[i].philo_thread, NULL);
		i++;
	}
	return (SUCCESS);
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

int	error_cleanup(
	t_table *table, char *msg_type, char *the_error, char *msg
	)
{
	error_msg(msg_type, the_error, msg);
	cleanup_table(table);
	return (ERROR);
}
