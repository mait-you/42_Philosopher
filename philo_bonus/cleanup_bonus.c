/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:06:35 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/15 15:31:33 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Free all resources used by the program
 * 
 * @param program The main program structure
 * @return 0 on success
 */
int	free_resources(t_program *program)
{
	unsigned int	i;

	if (!program)
		return (0);
	ft_pthread_mutex_destroy(program, &program->dead_lock);
	ft_pthread_mutex_destroy(program, &program->meal_lock);
	ft_pthread_mutex_destroy(program, &program->write_lock);
	i = -1;
	while (program->forks && ++i < program->table.num_of_philos)
		ft_pthread_mutex_destroy(program, &program->forks[i].fork_lock);
	if (program->philos)
	{
		free(program->philos);
		program->philos = NULL;
	}
	if (program->forks)
	{
		free(program->forks);
		program->forks = NULL;
	}
	return (0);
}

/**
 * Print error message with formatting
 * 
 * @param msg_type Type of message
 * @param the_error Error description
 * @param msg Additional message
 * @return 0 on success
 */
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
	return (0);
}

/**
 * Print error and cleanup resources
 * 
 * @param program The main program structure
 * @param msg_type Type of message
 * @param the_error Error description
 * @param msg Additional message
 * @return 1 for error
 */
int	error_cleanup(
	t_program *program, char *msg_type, char *the_error, char *msg
	)
{
	error_msg(msg_type, the_error, msg);
	free_resources(program);
	return (1);
}
