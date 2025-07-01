/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalyout <fbalyout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:59:55 by fbalyout          #+#    #+#             */
/*   Updated: 2025/06/27 19:35:23 by fbalyout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *string)
{
	size_t	count;

	count = 0;
	while (string[count])
		count++;
	return (count);
}

char	*ft_strdup(const char *string)
{
	int		i;
	int		size;
	char	*duplicate;

	i = 0;
	if (!string)
		return (NULL);
	size = ft_strlen(string);
	duplicate = malloc(sizeof(char) * (size + 1));
	if (!duplicate)
		return (0);
	while (string[i])
	{
		duplicate[i] = string[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len_s;
	size_t	i;

	len_s = ft_strlen((char *)src);
	i = 0;
	if (dstsize == 0)
		return (len_s);
	while (src[i] && i < (dstsize - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len_s);
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len_d;
	size_t	len_s;
	size_t	i;
	char	*source;

	source = (char *)src;
	len_d = ft_strlen(dest);
	len_s = ft_strlen(source);
	if (size <= len_d)
		return (size + len_s);
	i = 0;
	while (source[i] && (i + len_d + 1) < size)
	{
		dest[len_d + i] = source[i];
		i++;
	}
	dest[len_d + i] = '\0';
	return (len_d + len_s);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*new_s;
	size_t	s_len;

	if (!s)
		return (ft_strdup(""));
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	new_s = (char *)malloc((len + 1) * sizeof(char));
	if (!new_s)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_s[i] = s[start + i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
