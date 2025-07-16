#include "../minirt.h"
t_color	get_surface_color(t_point *point)
{
    if (point->obj->texture == CHECKER)
        return (point->color);
    else if (point->obj->texture == BUMP)
        return (get_texture_color(point, point->obj));
    else
        return (point->obj->color);
}

void	init_l_s(t_l_s *data)
{
    data->shininess = 500;
    data->specular_strength = 0.5;
    data->color.r = 0;
    data->color.g = 0;
    data->color.b = 0;
    data->lfar9 = 1.0f;
}

void	clamp_color(t_color *color)
{
    color->r = fmin(color->r, 255.0f);
    color->g = fmin(color->g, 255.0f);
    color->b = fmin(color->b, 255.0f);
}

int	pack_color(t_color color)
{
    uint32_t	a;

    a = 255;
    return (((int)color.r << 24) | ((int)color.g << 16) | 
            ((int)color.b << 8) | a);
}
void color_handle_help(t_minirt *data, t_l_s *s_l_data, t_color *obj, t_color *c_light)
{
    s_l_data->color.r += obj->r * c_light->r / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
    s_l_data->color.g += obj->g * c_light->g / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
    s_l_data->color.b += obj->b * c_light->b / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
}

