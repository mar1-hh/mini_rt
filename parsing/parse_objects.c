#include "../minirt.h"

int	check_texture_type(char *texture)
{
	if (ft_strncmp(texture, "checker", 7) == 0)
		return (CHECKER);
	else if (ft_strncmp(texture, "bump", 4) == 0)
		return (BUMP);
	else
		return (NONE);
}

void	handling_pump_tex(char **line, t_object *object)
{
	char	*path;

	if (object->texture == BUMP)
	{
		skip_space(line);
		path = ft_strdup_line(*line);
		object->bump_texture = mlx_load_png(path);
		if (!object->bump_texture)
		{
			printf("Error: Failed to load bump texture for plane: %s\n", path);
			object->texture = NONE;
		}
		free(path);
	}
	else
		object->bump_texture = NULL;
}

void	parse_plane(char *line, t_object *object)
{
	object->type = PLANE;
	if (line[0] == 'p' && line[1] == 'l')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin = parse_vec3(&line);
	skip_space(&line);
	object->normal = parse_vec3(&line);
	skip_space(&line);
	object->color.r = (float)atoi(line);
	skip_exept(&line, ',');
	object->color.g = (float)atoi(line);
	skip_exept(&line, ',');
	object->color.b = (float)atoi(line);   
	object->next = NULL;
	skip_space(&line);
	object->texture = check_texture_type(line);
	handling_pump_tex(&line, object);
}

void	parse_sphere(char *line, t_object *object, t_minirt *data)
{
	(void)data;
	object->type = SPHERE;
	if (line[0] == 's' && line[1] == 'p')
		line += 2;
	skip_space(&line);
	object->origin = parse_vec3(&line);
	while (*line && !ft_isspace(*line))
		line++;
	while (ft_isspace(*line))
		line++;
	object->diameter = ft_atof(line);
	while (*line && !ft_isspace(*line))
		line++;
	while (ft_isspace(*line))
		line++;
	object->color.r = (float)atoi(line);
	skip_exept(&line, ',');
	object->color.g = (float)atoi(line);
	skip_exept(&line, ',');
	object->color.b = (float)atoi(line);
	object->next = NULL;
	skip_space(&line);
	object->texture = check_texture_type(line);
	handling_pump_tex(&line, object);
}

void	parse_cylinder(char *line, t_object *object)
{
	object->type = CYLINDER;
	if (line[0] == 'c' && line[1] == 'y')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin = parse_vec3(&line);
	skip_space(&line);
	object->normal = parse_vec3(&line);
	skip_space(&line);
	object->diameter = ft_atof(line);
	skip_space(&line);
	object->height = ft_atof(line);
	skip_space(&line);
	object->color.r = (float)atoi(line);
	skip_exept(&line, ',');
	object->color.g = (float)atoi(line);
	skip_exept(&line, ',');
	object->color.b = (float)atoi(line);
	object->next = NULL;
	skip_space(&line);
	object->texture = check_texture_type(line);
	handling_pump_tex(&line, object);
}
