#include "../minirt.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>

int ft_isspace(char str)
{
	if (str == 32 || str == '\n' || str == '\t' || str == '\f' || str == '\f' || str == '\r')
		return 1;
	return 0;
}

void parse_ambient(char *line, t_ambient *ambient)
{
	
	if (*line == 'A')
		line++;
	ambient->ratio =  atof(line);
	while (ft_isspace(*line))
		line++;
	ambient->R = atoi(line);
	if (*line == ',')
		line++;
	ambient->G = atoi(line);
	if (*line == ',')
		line++;
	ambient->B = atoi(line);
}

void parse_light(char *line, t_light *light)
{
	if (*line == 'L')
		line++;
	while (ft_isspace(*line))
		line++;
	light->origin.x = atof(line);
	if (*line == ',')
		line++;
	light->origin.y = atof(line);
	if (*line == ',')
		line++;
	light->origin.z = atof(line);
	while (ft_isspace(*line))
		line++;
	light->ratio = atof(line);
	while (ft_isspace(*line))
		line++;
	light->R = atoi(line);
	if (*line == ',')
		line++;
	light->G = atoi(line);
	if (*line == ',')
		line++;
	light->B = atoi(line);
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
void parse_camera(char *line, t_camera *camera)
{
	if (*line == 'C')
		line++;
	while (ft_isspace(*line))
		line++;
	camera->origin.x = atof(line);
	skip_exept(&line, ',');
	camera->origin.y = atof(line);
	skip_exept(&line, ',');
	camera->origin.z = atof(line);
	skip_space(&line);
	camera->normal.x = atof(line);
	skip_exept(&line, ',');
	camera->normal.y = atof(line);
	skip_exept(&line, ',');
	camera->normal.z = atof(line);
	skip_space(&line);
	camera->fov = atof(line);
}

void parse_plane(char *line, t_object *object)
{
	object->type = PLANE;
	if (line[0] == 'p' && line[1] == 'l')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin.x = atof(line);
	skip_exept(&line, ',');
	object->origin.y = atof(line);
	skip_exept(&line, ',');
	object->origin.z = atof(line);
	skip_space(&line);
	object->normal.x = atof(line);
	skip_exept(&line, ',');
	object->normal.y = atof(line);
	skip_exept(&line, ',');
	object->normal.z = atof(line);
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
	object->origin.x = atof(line);
	skip_exept(&line, ',');
	object->origin.y = atof(line);
	skip_exept(&line, ',');
	object->origin.z = atof(line);
	skip_space(&line);
	object->diameter = atof(line);
	skip_space(&line);
	object->normal.x = atof(line);
	skip_exept(&line, ',');
	object->normal.y = atof(line);
	skip_exept(&line, ',');
	object->normal.z = atof(line);
	skip_space(&line);
	object->R = atoi(line);
	skip_exept(&line, ',');
	object->G = atoi(line);
	skip_exept(&line, ',');
	object->B = atoi(line);
}

void parse_file(char *filename, t_minirt *data)
{
	char *line;
	int fd;
	t_object *current;
	t_object *new;

	fd = open(filename, O_RDWR);
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
			parse_light(line, &data->light);
		if (*line == 'C')
			parse_camera(line, &data->camera);
		if ((line[0] == 'p' && line[1] == 'l')
			|| (line[0] == 'c' && line[1] == 'y')
			|| (line[0] == 's' && line[1] == 'p')) {
			new = malloc(sizeof(t_object));
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
		}
		if (line[0] == 'p' && line[1] == 'l')
			parse_plane(line, new);
		if (line[0] == 's' && line[1] == 'p')
			parse_sphere(line, new);
		if (line[0] == 'c' && line[1] == 'y')
			parse_cylinder(line, new);
	}
}
