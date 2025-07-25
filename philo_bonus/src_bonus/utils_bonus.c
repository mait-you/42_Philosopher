/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/25 09:02:54 by mait-you         ###   ########.fr       */
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

static void	format_message(char *str_time, char *str_id, char *status)
{
	int			i;
	int			j;
	char		*buffer;

	i = 0;
	j = 0;
	if (!str_time || !str_id)
		exit(error_msg("malloc", NULL, "Failed to allocate"));
	buffer = malloc(ft_strlen(str_time) + ft_strlen(str_id) \
		+ ft_strlen(status) + 2);
	if (!buffer)
		return (free(str_time), free(str_id),
			exit(error_msg("malloc", NULL, "Failed to allocate")));
	while (str_time && str_time[j])
		buffer[i++] = str_time[j++];
	buffer[i++] = ' ';
	j = 0;
	while (str_id && str_id[j])
		buffer[i++] = str_id[j++];
	j = 0;
	while (status && status[j])
		buffer[i++] = status[j++];
	buffer[i] = 0;
	ft_putstr_fd(buffer, STDOUT_FILENO);
	return (free(str_time), free(str_id), free(buffer));
}

void	print_status(t_philo *philo, t_state status)
{
	char		*color;
	const char	*str_status[] = {" has taken a fork"RESET, \
" is eating"RESET, " is sleeping"RESET, " is thinking"RESET, \
" died"RESET};

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
