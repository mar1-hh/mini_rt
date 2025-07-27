#include "minirt_bonus.h"

int    ft_strcmp(char *str1, char *str2)
{
    int i;

    i = 0;
    while (str1[i] || str2[i])
    {
        if (str1[i] > str2[i])
            return (1);
        else if (str1[i] < str2[i])
            return (-1);
        i++;
    }
    return (0);
}

void    my_mlx_p_pix(unsigned int color, int x, int y, t_minirt *data)
{
    char    *ptr;

    ptr = data->addr + (y * data->line_length + x * (data->bits_per_pexel / 8));
    *(unsigned int*)ptr = color;
}
int main(int ac, char **av)
{
    t_minirt    data;

    if (ac != 2)
    {
        printf("Usage: %s <file.rt>\n", av[0]);
        return (1);
    }
    if (!is_dotrt(av[1]))
    {
        printf("Error: File must have .rt extension\n");
        return (1);
    }
    init_data(&data);
    if (!data.mlx)
    {
        printf("Error: Failed to initialize MLX\n");
        return (1);
    }
    parse_file(av[1], &data);
    rays_setup(&data);
    mlx_loop(data.mlx);
    cleanup_data(&data);
    return (0);
}
