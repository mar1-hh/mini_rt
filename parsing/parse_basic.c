#include "../minirt.h"

void	parse_ambient(char *line, t_ambient *ambient)
{
	if (*line == 'A')
		line++;
	while (ft_isspace(*line))
		line++;
	ambient->ratio = ft_atof(line);
	skip_space(&line);
	ambient->color.r = (float)atoi(line);
	skip_exept(&line, ',');
	ambient->color.g = (float)atoi(line);
	skip_exept(&line, ',');
	ambient->color.b = (float)atoi(line);
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
	light->color.r = atoi(line);
	skip_exept(&line, ',');
	light->color.g = atoi(line);
	skip_exept(&line, ',');
	light->color.b = atoi(line);
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
