#include "../minirt.h"

int	ft_isspace(char str)
{
	if (str == 32 || str == '\n' || str == '\t' || str == '\f' || str == '\r')
		return (1);
	return (0);
}

void	skip_exept(char **line, char to_skip)
{
	while ((**line) && (**line) != to_skip)
		(*line)++;
	if ((**line) == to_skip)
		(*line)++;
}
void	skip_space(char **line)
{
	while (**line && **line != ' ')
		(*line)++;
	while (ft_isspace(**line))
		(*line)++;
}
t_vec3	parse_vec3(char **line)
{
	t_vec3	vec;

	vec = (t_vec3){0, 0, 0};
	vec.x = ft_atof(*line);
	skip_exept(line, ',');
	vec.y = ft_atof(*line);
	skip_exept(line, ',');
	vec.z = ft_atof(*line);
	return (vec);
}
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

char	*ft_strdup_line(char *line)
{
	int	i;
	int	j;
	char *ptr;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	ptr = ft_calloc(1, i + 1);
	if (!ptr)
		return (NULL);
	j = 0;
	while (j < i)
	{
		ptr[j] = line[j];
		j++;
	}
	return (ptr);
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

static void	parse_light_line(char *line, t_minirt *data)
{
	t_light	*new_light;

	new_light = malloc(sizeof(t_light));
	if (!new_light)
		return ;
	new_light->next = NULL;
	add_light_to_list(data, new_light);
	parse_light(line, new_light);
}

static void	parse_object_line(char *line, t_minirt *data)
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

static int	is_object_line(char *line)
{
	return ((line[0] == 'p' && line[1] == 'l')
		|| (line[0] == 'c' && line[1] == 'y')
		|| (line[0] == 's' && line[1] == 'p')
		|| (line[0] == 'c' && line[1] == 'o'));
}

static void	process_line(char *line, t_minirt *data)
{
	while (ft_isspace(*line))
		line++;
	if (*line == 'A')
		parse_ambient(line, &data->ambient);
	else if (*line == 'L')
		parse_light_line(line, data);
	else if (*line == 'C')
		parse_camera(line, &data->camera);
	else if (is_object_line(line))
		parse_object_line(line, data);
}

void	parse_file(char *filename, t_minirt *data)
{
	char	*line;
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		write(2, "failed\n", 7);
		return ;
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		process_line(line, data);
		free(line);
	}
	close(fd);
}
