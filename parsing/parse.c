#include "../minirt.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>

int ft_isspace(char str)
{
	if (str == 32 || str == '\n' || str == '\t' ||  str == '\f' || str == '\r')
		return 1;
	return 0;
}

void skip_exept(char **line, char to_skip)
{
	while ((**line) && (**line) != to_skip)
        (*line)++;
	if ((**line) == to_skip)
		(*line)++;
}
void skip_space(char **line)
{
	while (**line && **line != ' ')
        (*line)++;
	while (ft_isspace(**line))
		(*line)++;
}
void parse_ambient(char *line, t_ambient *ambient)
{
	
	if (*line == 'A')
		line++;
	while (ft_isspace(*line))
		line++;
	ambient->ratio =  ft_atof(line);
	skip_space(&line);
	ambient->R = atoi(line);
	skip_exept(&line, ',');
	ambient->G = atoi(line);
	skip_exept(&line, ',');
	ambient->B = atoi(line);
}

void parse_light(char *line, t_light *light)
{
	if (*line == 'L')
		line++;
	while (ft_isspace(*line))
		line++;
	light->origin.x = ft_atof(line);
	skip_exept(&line, ',');
	light->origin.y = ft_atof(line);
	skip_exept(&line, ',');
	light->origin.z = ft_atof(line);
	skip_space(&line);
	light->ratio = ft_atof(line);
	skip_space(&line);
	light->R = atoi(line);
	skip_exept(&line, ',');
	light->G = atoi(line);
	skip_exept(&line, ',');
	light->B = atoi(line);
}

void parse_camera(char *line, t_camera *camera)
{
	if (*line == 'C')
		line++;
	while (ft_isspace(*line))
		line++;
	camera->origin.x = ft_atof(line);
	skip_exept(&line, ',');
	camera->origin.y = ft_atof(line);
	skip_exept(&line, ',');
	camera->origin.z = ft_atof(line);
	skip_space(&line);
	camera->normal.x = ft_atof(line);
	skip_exept(&line, ',');
	camera->normal.y = ft_atof(line);
	skip_exept(&line, ',');
	camera->normal.z = ft_atof(line);
	skip_space(&line);
	camera->fov = ft_atof(line);
}

void parse_plane(char *line, t_object *object)
{
	object->type = PLANE;
	if (line[0] == 'p' && line[1] == 'l')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin.x = ft_atof(line);
	skip_exept(&line, ',');
	object->origin.y = ft_atof(line);
	skip_exept(&line, ',');
	object->origin.z = ft_atof(line);
	skip_space(&line);
	object->normal.x = ft_atof(line);
	skip_exept(&line, ',');
	object->normal.y = ft_atof(line);
	skip_exept(&line, ',');
	object->normal.z = ft_atof(line);
	skip_space(&line);
	object->R = atoi(line);
	skip_exept(&line, ',');
	object->G = atoi(line);
	skip_exept(&line, ',');
	object->B = atoi(line);
}

void parse_sphere(char *line, t_object *object)
{
    object->type = SPHERE;
    if (line[0] == 's' && line[1] == 'p')
        line += 2;
    while (ft_isspace(*line))
        line++;
    object->origin.x = ft_atof(line);
    skip_exept(&line, ',');
    object->origin.y = ft_atof(line);
    skip_exept(&line, ',');
    object->origin.z = ft_atof(line);
    while (*line && !ft_isspace(*line)) 
        line++;
    while (ft_isspace(*line))
        line++;
    object->diameter = ft_atof(line);
    while (*line && !ft_isspace(*line)) 
        line++;
    while (ft_isspace(*line))
        line++;
    object->R = ft_atoi(line);
    skip_exept(&line, ',');
    object->G = ft_atoi(line);
    skip_exept(&line, ',');
    object->B = ft_atoi(line);
    object->next = NULL;
}

void parse_cylinder(char *line, t_object *object)
{
	object->type = CYLINDER;
	if (line[0] == 'c' && line[1] == 'y')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin.x = ft_atof(line);
	skip_exept(&line, ',');
	object->origin.y = ft_atof(line);
	skip_exept(&line, ',');
	object->origin.z = ft_atof(line);
	skip_space(&line);
	object->normal.x = ft_atof(line);
	skip_exept(&line, ',');
	object->normal.y = ft_atof(line);
	skip_exept(&line, ',');
	object->normal.z = ft_atof(line);
	skip_space(&line);
	object->diameter = ft_atof(line);
	skip_space(&line);
	object->height = ft_atof(line);
	skip_space(&line);
	object->R = ft_atoi(line);
	skip_exept(&line, ',');
	object->G = ft_atoi(line);
	skip_exept(&line, ',');
	object->B = ft_atoi(line);
}

void parse_cone(char *line, t_object *object)
{
	object->type = CONE;
	if (line[0] == 'c' && line[1] == 'o')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin.x = ft_atof(line);
	skip_exept(&line, ',');
	object->origin.y = ft_atof(line);
	skip_exept(&line, ',');
	object->origin.z = ft_atof(line);
	skip_space(&line);
	object->normal.x = ft_atof(line);
	skip_exept(&line, ',');
	object->normal.y = ft_atof(line);
	skip_exept(&line, ',');
	object->normal.z = ft_atof(line);
	skip_space(&line);
	object->diameter = ft_atof(line);
	skip_space(&line);
	object->height = ft_atof(line);
	skip_space(&line);
	object->R = ft_atoi(line);
	skip_exept(&line, ',');
	object->G = ft_atoi(line);
	skip_exept(&line, ',');
	object->B = ft_atoi(line);
}

void parse_file(char *filename, t_minirt *data)
{
	char *line;
	int fd;
	t_object *current;
	t_object *new;
	t_light *c_light;
	t_light *new_light;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		write(2, "failed\n", 7);
		return;
	}
	while (69) {
		line = get_next_line(fd);
		if (!line)
			break;
		while (ft_isspace(*line))
			line++;
		if (*line == 'A')
			parse_ambient(line, &data->ambient);
		if (*line == 'L')
		{
			new_light = malloc(sizeof(t_light));
			if (!new_light)
				return;
			new_light->next = NULL;
			if (!data->light)
				data->light = new_light;
			else
			{
				c_light = data->light;
				while (c_light->next)
					c_light = c_light->next;
				c_light->next = new_light;
			}
			parse_light(line, new_light);
		}
		if (*line == 'C')
			parse_camera(line, &data->camera);
		if ((line[0] == 'p' && line[1] == 'l')
			|| (line[0] == 'c' && line[1] == 'y')
			|| (line[0] == 's' && line[1] == 'p')
			|| (line[0] == 'c' && line[1] == 'o'))
		{
			new = malloc(sizeof(t_object));
			if (!new)
				return;
			new->next = NULL;
			if (!data->objects)
				data->objects = new;
			else
			{
				current = data->objects;
				while (current->next)
					current = current->next;;
				current->next = new;
			}
			if (line[0] == 'p' && line[1] == 'l')
				parse_plane(line, new);
			if (line[0] == 's' && line[1] == 'p')
				parse_sphere(line, new);
			if (line[0] == 'c' && line[1] == 'y')
				parse_cylinder(line, new);
			if (line[0] == 'c' && line[1] == 'o')
				parse_cone(line, new);
		}
	}
}
