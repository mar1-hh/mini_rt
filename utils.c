#include "minirt.h"

void    cleanup_lights(t_light *light)
{
    t_light *current;
    t_light *next;
    
    current = light;
    while (current)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

void    cleanup_objects(t_object *objects)
{
    t_object *current;
    t_object *next;
    
    current = objects;
    while (current)
    {
        next = current->next;
        if (current->bump_texture)
            mlx_delete_texture(current->bump_texture);
        free(current);
        current = next;
    }
}

void    cleanup_data(t_minirt *data)
{
    if (data->objects)
        cleanup_objects(data->objects);
    if (data->light)
        cleanup_lights(data->light);
    if (data->img)
        mlx_delete_image(data->mlx, data->img);
    if (data->mlx)
        mlx_terminate(data->mlx);
}

int is_dotrt(char *str)
{
    if(ft_strcmp(&str[ft_strlen(str)-3], ".rt"))
        return 0;
    return  1;
}

void    init_data(t_minirt *data)
{
    data->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", 0);
    data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
    mlx_image_to_window(data->mlx, data->img, 0, 0);
    data->objects = NULL;
    data->light = NULL;
}
