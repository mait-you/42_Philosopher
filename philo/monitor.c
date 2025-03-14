/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:07:06 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/14 13:49:48 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_simulation_done(t_program *program)
{
    pthread_mutex_lock(&program->dead_lock);
    program->table.simulation_done = true;
    pthread_mutex_unlock(&program->dead_lock);
}

bool is_simulation_done(t_program *program)
{
    bool r;

    r = false;
    pthread_mutex_lock(&program->dead_lock);
    if (program->table.simulation_done == true)
        r = true;
    pthread_mutex_unlock(&program->dead_lock);
    return (r);
}

static bool is_philo_dead(t_philo *philo, time_t time_to_die)
{
	bool re;

	re = false;
    pthread_mutex_lock(&philo->program->meal_lock);
    if (get_time_in_ms(philo->program) - philo->last_meal >= time_to_die)
        re = true;
    pthread_mutex_unlock(&philo->program->meal_lock);
    return (re);
}

static int	check_if_dead(t_program *program, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < program->table.nb_philos)
	{
		if (is_philo_dead(&philos[i], program->table.time_to_die))
		{
            print_status(philos, DIED);
			pthread_mutex_lock(&program->dead_lock);
			philos->dead = 1;
			pthread_mutex_unlock(&program->dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_if_all_ate(t_program *program, t_philo *philos)
{
	unsigned int	i;
	unsigned int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].num_times_to_eat == -1)
		return (0);
	while (i < program->table.nb_philos)
	{
		pthread_mutex_lock(&program->meal_lock);
		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
			finished_eating++;
		pthread_mutex_unlock(&program->meal_lock);
		i++;
	}
	if (finished_eating == program->table.nb_philos)
	{
		pthread_mutex_lock(&program->dead_lock);
		philos->dead = 1;
		pthread_mutex_unlock(&program->dead_lock);
		return (1);
	}
	return (0);
}

void *monitor_routine(void *program_ptr)
{
    t_program *program;
    program = (t_program*)program_ptr;
    
    while (1)
    {
        if (check_if_dead(program, program->philos)|| 
            check_if_all_ate(program, program->philos))
        {
            set_simulation_done(program);
            break ;
        }
        usleep(1000);
    }
    return (NULL);
}
