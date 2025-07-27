#include "../minirt.h"

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
