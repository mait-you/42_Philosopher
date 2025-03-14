/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:07:06 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/12 16:07:12 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_simulation_done(t_program *program)
{
    ft_pthread_mutex_lock(program, &program->dead_lock);
    program->table.simulation_done = true;
    ft_pthread_mutex_unlock(program, &program->dead_lock);
}

bool is_simulation_done(t_program *program)
{
    bool r;

    r = false;
    ft_pthread_mutex_lock(program, &program->dead_lock);
    if (program->table.simulation_done == true)
        r = true;
    ft_pthread_mutex_unlock(program, &program->dead_lock);
    return (r);
}

static bool is_philo_dead(t_philo *philo, time_t time_to_die)
{
	bool re;

	re = false;
    ft_pthread_mutex_lock(philo->program, &philo->program->meal_lock);
    if (get_time_in_ms(philo->program) - philo->last_meal >= time_to_die)
        re = true;
    ft_pthread_mutex_unlock(philo->program, &philo->program->meal_lock);
    return (re);
}

int	check_if_dead(t_program *program, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos[0].table->nb_philos)
	{
		if (is_philo_dead(&philos[i], program->table.time_to_die))
		{
            print_status(philos, DIED);
			ft_pthread_mutex_lock(program, &program->dead_lock);
			philos->dead = 1;
			ft_pthread_mutex_unlock(program, &program->dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_if_all_ate(t_program *program, t_philo *philos)
{
	unsigned int	i;
	unsigned int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].num_times_to_eat == -1)
		return (0);
	while (i < philos[0].table->nb_philos)
	{
		ft_pthread_mutex_lock(program, &program->meal_lock);
		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
			finished_eating++;
		ft_pthread_mutex_unlock(program, &program->meal_lock);
		i++;
	}
	if (finished_eating == philos[0].table->nb_philos)
	{
		ft_pthread_mutex_lock(program, &program->dead_lock);
		philos->dead = 1;
		ft_pthread_mutex_unlock(program, &program->dead_lock);
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
        if (check_if_dead(program, program->philos) == 1 || 
            check_if_all_ate(program, program->philos) == 1)
        {
            set_simulation_done(program);
            break ;
        }
        usleep(1000);
    }
    return (NULL);
}
