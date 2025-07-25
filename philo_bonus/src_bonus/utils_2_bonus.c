/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-you <mait-you@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:20:39 by mait-you          #+#    #+#             */
/*   Updated: 2025/07/24 11:08:20 by mait-you         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/philo_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	l;

	l = 0;
	while (*(s++))
		l++;
	return (l);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*ptr;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	ptr = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!ptr)
		return (NULL);
	i = -1;
	while (s1 && s1[++i])
		ptr[i] = s1[i];
	j = -1;
	while (s2 && s2[++j])
		ptr[i + j] = s2[j];
	ptr[i + j] = '\0';
	free(s2);
	return (ptr);
}

static int	n_len(unsigned long n)
{
	int		l;

	l = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		l++;
	}
	return (l);
}

static void	ft_putnb(char *s, unsigned long n)
{
	if (n > 9)
		ft_putnb(s - 1, n / 10);
	*s = '0' + (n % 10);
}

char	*ft_itoa(unsigned long nb)
{
	int		len;
	char	*str;

	len = n_len(nb);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	ft_putnb(str + len - 1, nb);
	str[len] = '\0';
	return (str);
}
