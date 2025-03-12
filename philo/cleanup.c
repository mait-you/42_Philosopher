/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:49:20 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/12 14:40:53 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int free_resources(t_program *program)
{
	unsigned int	i;

	if (!program)
		return (0);
	// ft_pthread_mutex_destroy(program, &program->dead_lock);
	// ft_pthread_mutex_destroy(program, &program->meal_lock);
	// ft_pthread_mutex_destroy(program, &program->write_lock);
	i = 0;
	while (i < program->table.nb_philos)
	{
		// ft_pthread_mutex_destroy(program, &program->forks[i].fork_lock);
		i++;
	}
	return (0);
}

int error_msg(char *msg_type, char *the_error, char *msg)
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

int error_cleanup(t_program *program, char *msg_type, char *the_error, char *msg)
{
	error_msg(msg_type, the_error, msg);
	free_resources(program);
	return (1);
}
