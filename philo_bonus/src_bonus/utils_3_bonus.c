/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/25 09:03:00 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	l;

	if (!s)
		return (ft_putstr_fd("(null)", fd));
	l = 0;
	if (fd == -1)
		return ;
	while (s[l])
		l++;
	write(fd, s, l);
}

void	check_simulation_done(t_philo *philo)
{
	sem_wait(philo->table->stop_sem);
	sem_post(philo->table->stop_sem);
}
