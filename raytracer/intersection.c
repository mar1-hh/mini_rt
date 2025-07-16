#include "../minirt.h"

float intersect_sphere(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
    return intersect_sphere_unified(data->camera.origin, ray_direction, current);
}

float intersect_plane(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
    float denom = dot(current->normal, ray_direction);
    if (fabs(denom) < 1e-6)
        return -1;
    t_vec3 L = sub_vec(current->origin, data->camera.origin);
    float t = dot(current->normal, L) / denom;
    return (t < 0) ? -1.0f : t;
}

float intersect_cylinder(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
    return intersect_cylinder_unified(data->camera.origin, ray_direction, current);
}

float intersect_cone(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
    return intersect_cone_unified(data->camera.origin, ray_direction, current);
}
int	is_shadow(t_minirt *data, t_point point, t_vec3 light_dir_n,
    t_vec3 origin_light)
{
float		max_distance;
float		distance;
t_object	*obj;

max_distance = vec_length(sub_vec(origin_light, point.origin));
obj = data->objects;
while (obj)
{
    if (obj != point.obj)
    {
        distance = get_shadow_distance(data, point, light_dir_n, obj);
        if (distance > 0.0f && distance < max_distance)
            return (1);
    }
    obj = obj->next;
}
return (0);
}