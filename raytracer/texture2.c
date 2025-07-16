#include "../minirt.h"
void	calculate_sphere_coords(t_point *point, t_object *obj, float *u, float *v)
{
    t_vec3	hit;
    
    hit = normalize(sub_vec(point->origin, obj->origin));
    *u = 0.5 + atan2(hit.z, hit.x) / (2 * M_PI);
    *v = 0.5 - asin(hit.y) / M_PI;
    *u = fmax(0.0f, fmin(1.0f, *u));
    *v = fmax(0.0f, fmin(1.0f, *v));
}
void	calculate_plane_coords(t_point *point, t_object *obj, float *u, float *v)
{
    t_vec3	plane_normal;

    plane_normal = normalize(obj->normal);
    if (fabs(plane_normal.y) > 0.9f)
    {
        *u = fmod(point->origin.x * 0.5, 1.0);
        *v = fmod(point->origin.z * 0.5, 1.0);
    }
    else if (fabs(plane_normal.x) > 0.9f)
    {
        *u = fmod(point->origin.z * 0.5, 1.0);
        *v = fmod(point->origin.y * 0.5, 1.0);
    }
    else
    {
        *u = fmod(point->origin.x * 0.5, 1.0);
        *v = fmod(point->origin.y * 0.5, 1.0);
    }
    if (*u < 0) *u += 1.0;
    if (*v < 0) *v += 1.0;
}