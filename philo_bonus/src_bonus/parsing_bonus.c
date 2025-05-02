/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:37 by mait-you          #+#    #+#             */
/*   Updated: 2025/05/01 18:09:39 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	get_arg_as_num(const char *str)
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

static int	is_contains_non_digits(const char *av)
{
	while (av && *av)
	{
		if (*av < '0' || *av > '9')
			return (ERROR);
		av++;
	}
	return (SUCCESS);
}

int	parsing(int ac, char **av)
{
	int	i;
	int	num;

	if (ac != 5 && ac != 6)
		return (error_msg("Usage", NULL, ARGS_ERROR));
	i = 1;
	while (i < ac)
	{
		if (is_contains_non_digits(av[i]) == ERROR)
			return (error_msg("invalid arguments", av[i],
					"the argument must be unsigned \
integer between 0 and 2147483647"));
		num = get_arg_as_num(av[i]);
		if (num == -1)
			return (error_msg("invalid arguments", av[i],
					"the argument is more than 2147483647"));
		i++;
	}
	return (SUCCESS);
}
