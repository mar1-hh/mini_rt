#include "minirt.h"

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

void    init_data(t_minirt *data)
{
    mlx_set_setting(MLX_MAXIMIZED, true);
    data->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", 0);
    data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
    mlx_image_to_window(data->mlx, data->img, 0, 0);
    // data->mlx_window = mlx_new_window(data->mlx, WIDTH, HEIGHT, "minirt");
    // data->addr = mlx_get_data_addr(data->img, &data->bits_per_pexel, &data->line_length, &data->endian);
    data->objects = NULL;
    data->light = NULL;
}
int is_dotrt(char *str)
{
    if(ft_strcmp(&str[ft_strlen(str)-3], ".rt"))
        return 0;
    return  1;
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    t_minirt    data;

    init_data(&data);
    parse_file(av[1], &data);	
    rays_setup(&data);
    // mlx_put_image_to_window(data.mlx, data.mlx_window, data.img, 0, 0);
    // mlx_key_hook(data.mlx_window, key_code, &data);
    // mlx_loop_hook()
    // mlx_hook(data.mlx_window, 17, 0, close_window, &data);
    mlx_loop(data.mlx);
    mlx_terminate(data.mlx);
    return (0);
}
