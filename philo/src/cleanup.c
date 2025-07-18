/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:35 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/13 11:09:52 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	cleanup_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philos)
	{
		if (table->forks && table->forks_initialized-- > 0)
			pthread_mutex_destroy(&table->forks[i]);
		if (table->philos && table->philos[i].meal_lock_mutex_initialized)
			pthread_mutex_destroy(&table->philos[i].meal_lock_mutex);
		i++;
	}
	if (table->print_mutex_initialized)
		pthread_mutex_destroy(&table->print_mutex);
	if (table->simulation_mutex_initialized)
		pthread_mutex_destroy(&table->simulation_mutex);
	if (table->philos)
		free(table->philos);
	if (table->forks)
		free(table->forks);
	table->forks = NULL;
	table->philos = NULL;
	table->print_mutex_initialized = false;
	table->simulation_mutex_initialized = false;
	return (ERROR);
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

static void	ft_putstr_fd(char *s, int fd)
{
	size_t	l;

	if (!s)
		return (ft_putstr_fd("(null)", fd));
	l = 0;
	if (fd == -1)
		return ;
	while (s[l])
		l++;
	write(fd, s, l);
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

int	error_cleanup(
	t_table *table, char *msg_type, char *the_error, char *msg
	)
{
	error_msg(msg_type, the_error, msg);
	cleanup_table(table);
	return (ERROR);
}
