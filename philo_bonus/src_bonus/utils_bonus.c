/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/24 11:10:47 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ms_sleep(t_philo *philo, time_t time)
{
	time_t	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time)
		usleep(philo->table->sleep_chunk);
}

static int	format_message(char *str_time, char *str_id, char *status)
{
	int			i;
	int			j;
	char		*buffer;

	i = 0;
	j = 0;
	buffer = malloc(ft_strlen(str_time) + ft_strlen(str_id) \
		+ ft_strlen(status) + 2);
	if (!buffer)
		return (free(str_time), free(str_id), 0);
	while (str_time && str_time[j])
		buffer[i++] = str_time[j++];
	buffer[i++] = ' ';
	j = 0;
	while (str_id && str_id[j])
		buffer[i++] = str_id[j++];
	j = 0;
	while (status[j])
		buffer[i++] = status[j++];
	buffer[i] = '\0';
	free(str_time);
	free(str_id);
	write(STDOUT_FILENO, buffer, ft_strlen(buffer));
	free(buffer);
	return (i);
}

void	print_status(t_philo *philo, t_state status)
{
	char		*color;
	const char	*str_status[] = {" has taken a fork\n"RESET, \
" is eating\n"RESET, " is sleeping\n"RESET, " is thinking\n"RESET, \
" died\n"RESET};

	sem_wait(philo->table->print_sem);
	check_simulation_done(philo);
	color = NULL;
	if (status == TAKE_FORK)
		color = GRAYL;
	else if (status == EATING)
		color = GREEN;
	else if (status == SLEEPING)
		color = CYAN;
	else if (status == THINKING)
		color = YELLOW;
	else if (status == DIED)
	{
		color = RED;
		sem_wait(philo->table->stop_sem);
	}
	format_message(ft_strjoin(color, \
		ft_itoa(get_time_ms() - philo->table->simulation_start)), \
		ft_itoa(philo->id), (char *)str_status[status]);
	sem_post(philo->table->print_sem);
}

void	check_simulation_done(t_philo *philo)
{
	sem_wait(philo->table->stop_sem);
	sem_post(philo->table->stop_sem);
}
