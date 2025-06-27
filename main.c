#include "minirt.h"

void    my_mlx_p_pix(unsigned int color, int x, int y, t_minirt *data)
{
    char    *ptr;

    ptr = data->addr + (y * data->line_length + x * (data->bits_per_pexel / 8));
    *(unsigned int*)ptr = color;
}

void    init_data(t_minirt *data)
{
    data->mlx = mlx_init();
    data->img = mlx_new_image(data->mlx, 900, 900);
    data->mlx_window = mlx_new_window(data->mlx, 900, 900, "minirt");
    data->addr = mlx_get_data_addr(data->img, &data->bits_per_pexel, &data->line_length, &data->endian);
}


int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    t_minirt    data;

    init_data(&data);
    // my_mlx_p_pix(0x00FF0000, 450, 450, &data);
    draw_circle(&data, 450, 450, 100, 0x00FF0000);
    mlx_put_image_to_window(data.mlx, data.mlx_window, data.img, 0, 0);
    mlx_loop(data.mlx);
    return (0);
}
