#include "../minirt.h"

void	parse_cone(char *line, t_object *object)
{
	object->type = CONE;
	if (line[0] == 'c' && line[1] == 'o')
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
	object->color.r = (float)ft_atoi(line);
	skip_exept(&line, ',');
	object->color.g = (float)ft_atoi(line);
	skip_exept(&line, ',');
	object->color.b = (float)ft_atoi(line);
	object->next = NULL;
	skip_space(&line);
	object->texture = check_texture_type(line);
	handling_pump_tex(&line, object);
}

static void	add_light_to_list(t_minirt *data, t_light *new_light)
{
	t_light	*c_light;

	if (!data->light)
		data->light = new_light;
	else
	{
		c_light = data->light;
		while (c_light->next)
			c_light = c_light->next;
		c_light->next = new_light;
	}
}

static void	add_object_to_list(t_minirt *data, t_object *new)
{
	t_object	*current;

	if (!data->objects)
		data->objects = new;
	else
	{
		current = data->objects;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

void	parse_light_line(char *line, t_minirt *data)
{
	t_light	*new_light;

	new_light = malloc(sizeof(t_light));
	if (!new_light)
		return ;
	new_light->next = NULL;
	add_light_to_list(data, new_light);
	parse_light(line, new_light);
}

void	parse_object_line(char *line, t_minirt *data)
{
	t_object	*new;

	new = malloc(sizeof(t_object));
	if (!new)
		return ;
	new->next = NULL;
	add_object_to_list(data, new);
	if (line[0] == 'p' && line[1] == 'l')
		parse_plane(line, new);
	else if (line[0] == 's' && line[1] == 'p')
		parse_sphere(line, new, data);
	else if (line[0] == 'c' && line[1] == 'y')
		parse_cylinder(line, new);
	else if (line[0] == 'c' && line[1] == 'o')
		parse_cone(line, new);
}
