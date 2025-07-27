#include "get_next_line_bonus.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len_s1;
	int		len_s2;
	char	*string;

	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		return (ft_strdup(s1));
	if (!s1)
		return (ft_strdup(s2));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	string = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!string)
		return (NULL);
	ft_strlcpy(string, s1, len_s1 + 1);
	ft_strlcat(string, s2, len_s1 + len_s2 + 1);
	return (string);
}

char	*ft_strchr_or_free_rem(const char *s, int c, char **reminder)
{
	int		i;
	char	*string;
	char	ch;

	if (!s && c == 0) 
	{
		free(*reminder);
		*reminder = NULL;
		return (NULL);
	}
	else 
	{
		i = 0;
		string = (char *)s;
		ch = (char)c;
		while (string[i] || ch == '\0')
		{
			if (string[i] == ch)
				return (&string[i]);
			i++;
		}
		return (NULL);
	}
}

static char	*fonction_01(int fd, char *buffer, char *reminder)
{
	char	*temp;
	int		readen_bytes;

	readen_bytes = 1;
	while (readen_bytes > 0)
	{
		readen_bytes = read(fd, buffer, BUFFER_SIZE);
		if (readen_bytes == -1)
			return (NULL);
		else if (readen_bytes == 0)
			break ;
		buffer[readen_bytes] = '\0';
		if (!reminder)
			reminder = ft_strdup("");
		if (!reminder)
			return (NULL);
		temp = reminder;
		reminder = ft_strjoin(temp, buffer);
		free(temp);
		if (!reminder)
			return (NULL);
		if (ft_strchr_or_free_rem(buffer, '\n', NULL))
			break ;
	}
	return (reminder);
}

static char	*set_reminder(char *line)
{
	char	*reminder;
	size_t	i;
	size_t	k;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0' || line[i + 1] == '\0')
		return (0);
	k = ft_strlen(line);
	reminder = ft_substr(line, i + 1, k - i - 1);
	if (!reminder || !*reminder) 
		return (ft_strchr_or_free_rem(NULL, 0, &reminder));
	line[i + 1] = '\0';
	return (reminder);
}

char	*get_next_line(int fd)
{
	static char	*reminder;
	char		*buffer;
	char		*line;
	char		*new_line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0))
		return (ft_strchr_or_free_rem(NULL, 0, &reminder));
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_strchr_or_free_rem(NULL, 0, &reminder));
	line = fonction_01(fd, buffer, reminder);
	free(buffer);
	if (!line)
		return ((ft_strchr_or_free_rem(NULL, 0, &reminder)), NULL);
	reminder = set_reminder(line);
	new_line = ft_strdup(line);
	if (!new_line)
		(ft_strchr_or_free_rem(NULL, 0, &reminder));
	free(line);
	return (new_line);
}
