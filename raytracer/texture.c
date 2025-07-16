#include "../minirt.h"

void calculate_cylinder_coords(t_point *point, t_object *obj, float *u, float *v)
{
    t_vec3 local_hit = sub_vec(point->origin, obj->origin);
    float theta = atan2(local_hit.z, local_hit.x);
    *u = (theta + M_PI) / (2 * M_PI);
    *v = local_hit.y * 0.1;
    *v = fmod(*v, 1.0);
    if (*v < 0) *v += 1.0;
}

void calculate_cone_coords(t_point *point, t_object *obj, float *u, float *v)
{
    t_vec3 axis = normalize(obj->normal);
    t_vec3 apex = add_vec(obj->origin, mul_vec(axis, obj->height));
    t_vec3 local_hit = sub_vec(point->origin, apex);
    
    float theta = atan2(local_hit.z, local_hit.x);
    *u = (theta + M_PI) / (2 * M_PI);
    
    float dist_from_apex = vec_length(local_hit);
    *v = dist_from_apex / (obj->height * 1.41421f);
    
    *u = fmax(0.0f, fmin(1.0f, *u));
    *v = fmax(0.0f, fmin(1.0f, *v));
}
void	calculate_texture_coords(t_point *point, t_object *obj, float *u, float *v)
{
    if (obj->type == SPHERE)
        calculate_sphere_coords(point, obj, u, v);
    else if (obj->type == PLANE)
        calculate_plane_coords(point, obj, u, v);
    else if (obj->type == CYLINDER)
        calculate_cylinder_coords(point, obj, u, v);
    else if (obj->type == CONE)
        calculate_cone_coords(point, obj, u, v);
    else
    {
        *u = 0.0f;
        *v = 0.0f;
    }
}
t_color	sample_texture(mlx_texture_t *texture, float u, float v)
{
    int		tex_x;
    int		tex_y;
    uint8_t	*pixels;
    int		index;
    t_color	texture_color;

    tex_x = (int)(u * texture->width);
    tex_y = (int)(v * texture->height);
    tex_x = fmax(0, fmin(tex_x, texture->width - 1));
    tex_y = fmax(0, fmin(tex_y, texture->height - 1));
    pixels = texture->pixels;
    index = (tex_y * texture->width + tex_x) * 4;
    texture_color.r = pixels[index];
    texture_color.g = pixels[index + 1];
    texture_color.b = pixels[index + 2];
    return (texture_color);
}
t_color	get_texture_color(t_point *point, t_object *obj)
{
    float	u;
    float	v;

    if (!obj->bump_texture)
        return (obj->color);
    calculate_texture_coords(point, obj, &u, &v);
    return (sample_texture(obj->bump_texture, u, v));
}
