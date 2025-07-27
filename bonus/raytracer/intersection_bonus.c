#include "../minirt_bonus.h"

float	intersect_sphere(t_minirt *data, t_vec3 ray_direction,
		t_object *current)
{
	return (intersect_sphere_unified(data->camera.origin, ray_direction,
			current));
}

float	intersect_plane(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	float	denom;
	t_vec3	L;
	float	t;

	denom = dot(current->normal, ray_direction);
	if (fabs(denom) < 1e-6)
		return (-1);
	L = sub_vec(current->origin, data->camera.origin);
	t = dot(current->normal, L) / denom;
	return ((t < 0) ? -1.0f : t);
}

float	intersect_cylinder(t_minirt *data, t_vec3 ray_direction,
		t_object *current)
{
	return (intersect_cylinder_unified(data->camera.origin, ray_direction,
			current));
}

float	intersect_cone(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	return (intersect_cone_unified(data->camera.origin, ray_direction,
			current));
}
