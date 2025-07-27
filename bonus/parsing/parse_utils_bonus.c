#include "../minirt_bonus.h"

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
