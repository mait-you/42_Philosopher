/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:55 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/15 15:13:06 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Set the simulation as done
 *
 * @param program The main program structure
 */
void	set_simulation_done(t_program *program)
{
	pthread_mutex_lock(&program->dead_lock);
	program->table.simulation_done = true;
	pthread_mutex_unlock(&program->dead_lock);
}

/**
 * Check if the simulation is done
 *
 * @param program The main program structure
 * @return true if done, false otherwise
 */
bool	is_simulation_done(t_program *program)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&program->dead_lock);
	if (program->table.simulation_done == true)
		r = true;
	pthread_mutex_unlock(&program->dead_lock);
	return (r);
}

/**
 * Print the status of a philosopher with timestamp
 * 
 * @param philo Philosopher
 * @param status Status to print
 */
void	print_status(t_philo *philo, t_state status)
{
	time_t	current_time;

	if (is_simulation_done(philo->program) && status != DIED)
		return ;
	pthread_mutex_lock(&philo->program->write_lock);
	current_time
		= get_time_in_ms(philo->program) - philo->table->simulation_start;
	if (status == TAKE_FORK)
		printf("%ld %u has taken a fork\n", current_time, philo->id);
	else if (status == EATING)
		printf("%ld %u is eating\n", current_time, philo->id);
	else if (status == SLEEPING)
		printf("%ld %u is sleeping\n", current_time, philo->id);
	else if (status == THINKING)
		printf("%ld %u is thinking\n", current_time, philo->id);
	else if (status == DIED)
		printf("%ld %u died\n", current_time, philo->id);
	pthread_mutex_unlock(&philo->program->write_lock);
}
