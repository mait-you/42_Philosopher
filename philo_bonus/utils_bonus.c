/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:55 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/15 15:32:12 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

time_t	get_time_in_ms(t_program *program)
{
	struct timeval	tv;
	int				re;

	re = gettimeofday(&tv, NULL);
	if (re == EFAULT)
		return (error_cleanup(program, NULL, "get_time_in_ms",
				"An argument address referenced invalid memory."), -1);
	if (re == EPERM)
		return (error_cleanup(program, NULL, "get_time_in_ms",
				"A user other than the super-user attempted to set the \
time."), -1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	smart_usleep(t_program *program, time_t ms)
{
	time_t	start;

	start = get_time_in_ms(program);
	while ((get_time_in_ms(program) - start) <= ms)
	{
		if (is_simulation_done(program))
			return (0);
		usleep(100);
	}
	return (0);
}

int	ft_pthread_join(t_program *program, pthread_t *thread)
{
	int	re;

	re = pthread_join(*thread, NULL);
	if (re == EINVAL)
		return (error_cleanup(program, NULL, "ft_pthread_join",
				"The implementation has detected that the value specified by \
thread does not refer to a joinable thread."));
	if (re == ESRCH)
		return (error_cleanup(program, NULL, "ft_pthread_join",
				"No thread could be found corresponding to that specified by \
the given thread ID, thread."));
	if (re == EDEADLK)
		return (error_cleanup(program, NULL, "ft_pthread_join",
				"A deadlock was detected or the value of thread specifies the \
calling thread."));
	return (0);
}

int	ft_pthread_create(
	t_program *program, pthread_t *thread,
	void *(*start_routine)(void *), void *arg
	)
{
	int	re;

	re = pthread_create(thread, NULL, start_routine, arg);
	if (re == EAGAIN)
		return (error_cleanup(program, NULL, "ft_pthread_create",
				"pthread_create: The system lacked the necessary resources to \
create another thread, or the system-imposed limit on the total number of \
threads in a process."));
	if (re == EPERM)
		return (error_cleanup(program, NULL, "ft_pthread_create",
				"The caller does not have appropriate permission to set the \
required scheduling parameters or scheduling pol-icy."));
	if (re == EINVAL)
		return (error_cleanup(program, NULL, "ft_pthread_create",
				"The value specified by attr is invalid."));
	return (0);
}

int	ft_pthread_mutex_init(t_program *program, t_mtx *mutex)
{
	int	re;

	re = pthread_mutex_init(mutex, NULL);
	if (re == EINVAL)
		return (error_cleanup(program, NULL, "ft_pthread_mutex_init",
				"The value specified by attr is invalid."));
	if (re == EPERM)
		return (error_cleanup(program, NULL, "ft_pthread_mutex_init",
				"The process cannot allocate enough memory to create another \
mutex."));
	return (0);
}
