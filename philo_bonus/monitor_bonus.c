/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:07:06 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/15 15:31:41 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Check if all philosophers have eaten enough
 *
 * @param program The main program structure
 * @param philos Array of philosophers
 * @return 1 if all ate enough, 0 otherwise
 */
static int	waiter(t_program *program, t_philo *philos)
{
	unsigned int	i;
	unsigned int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (program->table.must_eat_count == -1)
		return (0);
	while (i < program->table.num_of_philos)
	{
		pthread_mutex_lock(&program->meal_lock);
		if (philos[i].num_times_to_eat
			>= (unsigned)program->table.must_eat_count)
			finished_eating++;
		pthread_mutex_unlock(&program->meal_lock);
		i++;
	}
	if (finished_eating == program->table.num_of_philos)
		return (set_simulation_done(program), 1);
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
	bool	re;
	time_t	current_time;

	re = false;
	pthread_mutex_lock(&philo->program->meal_lock);
	current_time = get_time_in_ms(philo->program);
	if ((current_time - philo->last_meal) >= time_to_die)
		re = true;
	pthread_mutex_unlock(&philo->program->meal_lock);
	return (re);
}

/**
 * Check if any philosopher has died
 *
 * @param program The main program structure
 * @param philos Array of philosophers
 * @return 1 if a philosopher died, 0 otherwise
 */
static int	angel_of_death(t_program *program, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < program->table.num_of_philos)
	{
		if (is_philo_dead(&philos[i], program->table.time_to_die))
		{
			print_status(&philos[i], DIED);
			set_simulation_done(program);
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
void	*monitor_routine(void *program_ptr)
{
	t_program	*program;

	program = (t_program *)program_ptr;
	while (!is_simulation_done(program))
	{
		if (angel_of_death(program, program->philos)
			|| waiter(program, program->philos))
			break ;
	}
	return (NULL);
}
