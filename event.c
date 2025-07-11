#include "minirt.h"

int close_window(t_minirt *data)
{
    mlx_destroy_window(data->mlx, data->mlx_window);
    exit(0);
    return (0);
}

int key_code(int code, t_minirt *data)
{
    if (code == 53)
    {
        mlx_destroy_window(data->mlx, data->mlx_window);
        exit(0);
    }
    return (0);
}
