#include "../minirt.h"
#include <sys/fcntl.h>
#include <unistd.h>

int is_space(char str)
{
	if (str == 32 || str == '\n' || str == '\t' || str == '\f' || str == '\f' || str == '\r')
		return 1;
	return 0;
}

void parse_ambient(char *line, t_ambient *ambient)
{
	while (line) {
		if (*line == 'A')
			line++;s
		
	}
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
		while (ft_isspace(line))
			line++;
		if (*line == 'A') {
			parse_ambient
		}
		if (*line == 'L') {
			parse_light
		}
		if (*line == 'C') {
			parse_camera
		}
		if (line[0] == 'p' && line[1] == 'l') {
			parse_plane
		}
		if (line[0] == 's' && line[1] == 'p') {
			parse_sphere
		}
		if (line[0] == 'c' && line[1] == 'y') {
			parse_cylinder
		}

	}
}
