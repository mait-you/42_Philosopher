/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:37 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/13 09:08:37 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	get_arg_as_num(char *str)
{
	int		i;
	long	r;

	i = 0;
	r = 0;
	if (str && str[i] == '+')
		i++;
	while (str && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - '0');
		if (r > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)(r));
}

static int	is_contains_non_digits(char **av)
{
	int	i;

	while ((**av >= 9 && **av <= 13) || **av == 32)
		(*av)++;
	i = 0;
	if ((*av)[i] == '+')
		i++;
	if ((*av)[i] == '-')
		return (ERROR);
	if (!(*av)[i])
		return (ERROR);
	while ((*av)[i] >= '0' && (*av)[i] <= '9')
		i++;
	while (((*av)[i] >= 9 && (*av)[i] <= 13) || (*av)[i] == 32)
		i++;
	if ((*av)[i])
		return (ERROR);
	return (SUCCESS);
}

int	parsing(int ac, char **av)
{
	int	i;
	int	num;

	if (ac != 5 && ac != 6)
		return (error_msg("Usage", NULL,
				"./philo <number_of_philosophers> <time_to_die> <time_to_eat> \
<time_to_sleep> [number_of_times_each_philosopher_must_eat]"));
	i = 1;
	while (i < ac && av[i])
	{
		if (is_contains_non_digits(&av[i]) == ERROR)
			return (error_msg("invalid arguments", av[i],
					"the argument must be integer between 0 and 2147483647"));
		num = get_arg_as_num(av[i]);
		if (num == -1)
			return (error_msg("invalid arguments", av[i],
					"the argument is more than 2147483647"));
		if (i == 1 && num > 200)
			return (error_msg("invalid arguments", av[i],
					"the argument is more than 200"));
		i++;
	}
	return (SUCCESS);
}
