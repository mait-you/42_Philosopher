/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:07:06 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/14 16:18:12 by mait-you         ###   ########.fr       */
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
 * Check if all philosophers have eaten enough
 *
 * @param program The main program structure
 * @param philos Array of philosophers
 * @return 1 if all ate enough, 0 otherwise
 */
static int	check_if_all_ate(t_program *program, t_philo *philos)
{
	unsigned int    i;
	unsigned int    finished_eating;
	int             meals_eaten;

	i = 0;
	finished_eating = 0;
	if (program->table.must_eat_count == -1)
		return (0);
	while (i < program->table.num_of_philos)
	{
		pthread_mutex_lock(&program->meal_lock);
		meals_eaten = philos[i].meals_eaten;
		pthread_mutex_unlock(&program->meal_lock);
		if (meals_eaten >= program->table.must_eat_count)
			finished_eating++;
		i++;
	}
	if (finished_eating == program->table.num_of_philos)
	{
		set_simulation_done(program);
		return (1);
	}
	return (0);
}

/**
 * Check if a philosopher has died
 *
 * @param philo Philosopher to check
 * @param time_to_die Time limit before death
 * @return true if dead, false otherwise
 */
static bool	is_philo_dead(t_philo *philo, time_t time_to_die)
{
	bool	result;
	time_t	last_meal_time;
	time_t	current_time;

	result = false;
	pthread_mutex_lock(&philo->program->meal_lock);
	last_meal_time = philo->last_meal;
	pthread_mutex_unlock(&philo->program->meal_lock);
	current_time = get_time_in_ms(philo->program);
	if (current_time - last_meal_time >= time_to_die)
		result = true;
	return (result);
}

/**
 * Check if any philosopher has died
 *
 * @param program The main program structure
 * @param philos Array of philosophers
 * @return 1 if a philosopher died, 0 otherwise
 */
static int	check_if_dead(t_program *program, t_philo *philos)
{
	unsigned int    i;

	i = 0;
	while (i < program->table.num_of_philos)
	{
		if (is_philo_dead(&philos[i], program->table.time_to_die))
		{
			print_status(&philos[i], DIED);
			set_simulation_done(program);
			pthread_mutex_lock(&program->dead_lock);
			philos[i].dead = 1;
			pthread_mutex_unlock(&program->dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * Monitor routine to check if philosophers die or all ate enough
 *
 * @param program_ptr Pointer to the program structure
 * @return NULL
 */
void *monitor_routine(void *program_ptr)
{
	t_program *program;
	program = (t_program*)program_ptr;
	usleep(1000);
	while (1)
	{
		if (check_if_dead(program, program->philos) ||
			check_if_all_ate(program, program->philos))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
