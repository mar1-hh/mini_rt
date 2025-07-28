/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:14:11 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/27 19:14:12 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

int	is_valide_float(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (str[i + 1] == '.' && !flag)
		{
			flag = 1;
			i++;
		}
		i++;
	}
	return (!str[i]);
}

double	help_atof(char *str)
{
	long	num;
	int		counter;
	double	res;

	num = 0;
	counter = 0;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		counter++;
		str++;
	}
	res = num * pow(10, -counter);
	return (res);
}

double	ft_atof(char *str)
{
	double	num;
	int		sign;

	num = 0;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		str++;
	}
	if (*str == '.')
		num += help_atof(++str);
	return (num * sign);
}
