/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:09:56 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/29 14:47:09 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_strcmp(char *str1, char *str2)
{
	int	i;

	i = 0;
	while (str1[i] || str2[i])
	{
		if (str1[i] > str2[i])
			return (1);
		else if (str1[i] < str2[i])
			return (-1);
		i++;
	}
	return (0);
}

void	handle_key(mlx_key_data_t keydata, void *param)
{
	t_minirt	*data;

	data = (t_minirt *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(data->mlx);
}

int	main(int ac, char **av)
{
	t_minirt	data;

	if (ac != 2)
	{
		printf("Usage: %s <file.rt>\n", av[0]);
		return (1);
	}
	if (!is_dotrt(av[1]))
	{
		printf("Error: File must have .rt extension\n");
		return (1);
	}
	init_data(&data);
	if (!data.mlx)
	{
		printf("Error: Failed to initialize MLX\n");
		return (1);
	}
	parse_file(av[1], &data);
	rays_setup(&data);
	mlx_key_hook(data.mlx, handle_key, &data);
	mlx_loop(data.mlx);
	cleanup_data(&data);
	return (0);
}
