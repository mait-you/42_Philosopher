/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:37 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/12 16:09:40 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_arg_as_num(char *str)
{
	int		i;
	long	r;

	i = 0;
	r = 0;
	while (str && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - '0');
		if (r > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)(r));
}

static bool	is_contains_non_digits(char *av)
{
	while (av && *av)
	{
		if (*av < '0' || *av > '9')
			return (true);
		av++;
	}
	return (false);
}

bool	parsing(int ac, char **av)
{
	int	i;
	int	num;

	i = 1;
	while (i < ac)
	{
		if (is_contains_non_digits(av[i]))
			return(error_msg("invalid arguments", av[i], PAR_MSG_1), false);
		num = get_arg_as_num(av[i]);
		if (i == 1 && (num == -1 || num > MAX_PHILO))
			return(error_msg("invalid arguments", av[i], PAR_MSG_2), false);
		if (num == -1)
			return(error_msg("invalid arguments", av[i], PAR_MSG_3), false);
		i++;
	}
	return (true);
}
