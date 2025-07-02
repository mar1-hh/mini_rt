#include "../minirt.h"
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

void parse_camera(char *line, t_camera *camera)
{
	if (*line == 'C')
		line++;
	while (ft_isspace(*line))
		line++;
	camera->origin.x = atof(line);
	if (*line == ',')
		line++;
	camera->origin.y = atof(line);
	if (*line == ',')
		line++;
	camera->origin.z = atof(line);
	while (ft_isspace(*line))
		line++;
	camera->normal.x = atof(line);
	if (*line == ',')
		line++;
	camera->normal.y = atof(line);
	if (*line == ',')
		line++;
	camera->normal.z = atof(line);
	while (ft_isspace(*line))
		line++;
	camera->fov = atof(line);
}

void parse_plane(char *line, t_object *object)
{
	if (line[0] == 'p' && line[1] == 'l')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin.x = atof(line);
	if (*line == ',')
		line++;
	object->origin.y = atof(line);
	if (*line == ',')
		line++;
	object->origin.z = atof(line);
	while (ft_isspace(*line))
		line++;
	object->normal.x = atof(line);
	if (*line == ',')
		line++;
	object->normal.y = atof(line);
	if (*line == ',')
		line++;
	object->normal.z = atof(line);
	while (ft_isspace(*line))
		line++;
	object->R = atoi(line);
	if (*line == ',')
		line++;
	object->G = atoi(line);
	if (*line == ',')
		line++;
	object->B = atoi(line);
}

void parse_sphere(char *line, t_object **object)
{
	if (line[0] == 's' && line[1] == 'p')
		line += 2;
	while (ft_isspace(*line))
		line++;
	float l = ft_atof(line);

	printf("%s\n", line);
	object->origin.x = l;
	if (*line == ',')
		line++;
	object->origin.y = atof(line);
	if (*line == ',')
		line++;
	object->origin.z = atof(line);
	while (ft_isspace(*line))
		line++;
	object->diameter = atof(line);
	while (ft_isspace(*line))
		line++;
	object->R = atoi(line);
	if (*line == ',')
		line++;
	object->G = atoi(line);
	if (*line == ',')
		line++;
	object->B = atoi(line);
}

void parse_cylinder(char *line, t_object *object)
{
	if (line[0] == 'c' && line[1] == 'y')
		line += 2;
	while (ft_isspace(*line))
		line++;
	object->origin.x = atof(line);
	if (*line == ',')
		line++;
	object->origin.y = atof(line);
	if (*line == ',')
		line++;
	object->origin.z = atof(line);
	while (ft_isspace(*line))
		line++;
	object->diameter = atof(line);
	while (ft_isspace(*line))
		line++;
	object->normal.x = atof(line);
	if (*line == ',')
		line++;
	object->normal.y = atof(line);
	if (*line == ',')
		line++;
	object->normal.z = atof(line);
	while (ft_isspace(*line))
		line++;
	object->R = atoi(line);
	if (*line == ',')
		line++;
	object->G = atoi(line);
	if (*line == ',')
		line++;
	object->B = atoi(line);
}

void parse_file(char *filename, t_minirt *data)
{
	char *line;
	int fd;

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
		if (*line == 'A') {
			parse_ambient(line, &data->ambient);
		}
		if (*line == 'L') {
			parse_light(line, &data->light);
		}
		if (*line == 'C') {
			parse_camera(line, &data->camera);
		}
		if (line[0] == 'p' && line[1] == 'l') {
			parse_plane(line, data->objects);
		}
		if (line[0] == 's' && line[1] == 'p') {
			parse_sphere(line, &data->objects);
		}
		if (line[0] == 'c' && line[1] == 'y') {
			parse_cylinder(line, data->objects);
		}

	}
}
