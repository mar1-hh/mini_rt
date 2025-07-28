/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_basic_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:38:32 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/28 17:38:34 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

void	parse_ambient(char *line, t_ambient *ambient)
{
	if (*line == 'A')
		line++;
	while (ft_isspace(*line))
		line++;
	ambient->ratio = ft_atof(line);
	skip_space(&line);
	ambient->color.r = (float)ft_atoi(line);
	skip_exept(&line, ',');
	ambient->color.g = (float)ft_atoi(line);
	skip_exept(&line, ',');
	ambient->color.b = (float)ft_atoi(line);
}

void	parse_light(char *line, t_light *light)
{
	if (*line == 'L')
		line++;
	while (ft_isspace(*line))
		line++;
	light->origin = parse_vec3(&line);
	skip_space(&line);
	light->ratio = ft_atof(line);
	skip_space(&line);
	light->color.r = ft_atoi(line);
	skip_exept(&line, ',');
	light->color.g = ft_atoi(line);
	skip_exept(&line, ',');
	light->color.b = ft_atoi(line);
}

void	parse_camera(char *line, t_camera *camera)
{
	if (*line == 'C')
		line++;
	while (ft_isspace(*line))
		line++;
	camera->origin = parse_vec3(&line);
	skip_space(&line);
	camera->normal = parse_vec3(&line);
	skip_space(&line);
	camera->fov = ft_atof(line);
}
