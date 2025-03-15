/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:09:37 by mait-you          #+#    #+#             */
/*   Updated: 2025/03/15 15:31:43 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/**
 * Convert string argument to integer
 * 
 * @param str String to convert
 * @return Integer value or -1 if invalid
 */
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

/**
 * Check if a string contains only digits
 * 
 * @param av String to check
 * @return true if contains non-digits, false otherwise
 */
static bool	is_contains_non_digits(const char *av)
{
	while (av && *av)
	{
		if (*av < '0' || *av > '9')
			return (true);
		av++;
	}
	return (false);
}

/**
 * Parse and validate command line arguments
 * 
 * @param ac Argument count
 * @param av Argument vector
 * @return true if valid, false otherwise
 */
bool	parsing(int ac, char **av)
{
	int	i;
	int	num;

	i = 1;
	while (i < ac)
	{
		if (is_contains_non_digits(av[i]))
			return (error_msg("invalid arguments", av[i],
					"the argument must be unsigned \
integer between 0 and 2147483647"),
				false);
		num = get_arg_as_num(av[i]);
		if (i == 1 && (num == -1 || num > MAX_PHILO))
			return (error_msg("invalid arguments", av[i],
					"<number_of_philosophers> is more than 200"), false);
		if (num == -1)
			return (error_msg("invalid arguments", av[i],
					"the argument is more than 2147483647"), false);
		i++;
	}
	return (true);
}
