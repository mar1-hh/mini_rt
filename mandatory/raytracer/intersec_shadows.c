#include "../minirt.h"

float	intersect_sphere_shadow(t_vec3 ray_origin, t_vec3 ray_direction,
		t_object *sphere)
{
	return (intersect_sphere_unified(ray_origin, ray_direction, sphere));
}

float	intersect_cylinder_shadow(t_vec3 ray_origin, t_vec3 ray_direction,
		t_object *current)
{
	return (intersect_cylinder_unified(ray_origin, ray_direction, current));
}

float	intersect_cone_shadow(t_vec3 ray_origin, t_vec3 ray_direction,
		t_object *current)
{
	return (intersect_cone_unified(ray_origin, ray_direction, current));
}

float	intersect_plane_shadow(t_minirt *data, t_vec3 ray_direction,
		t_object *current)
{
	float	denom;
	t_vec3	L;
	float	t;

	denom = dot(current->normal, ray_direction);
	if (fabs(denom) < 1e-6)
		return (-1);
	L = sub_vec(current->origin, data->camera.origin);
	t = dot(current->normal, L) / denom;
	if (t < 0)
		return (-1.0f);
	return (t);
}
float	get_shadow_distance(t_minirt *data, t_point point, t_vec3 light_dir_n,
		t_object *obj)
{
	float	distance;

	if (obj->type == SPHERE)
		distance = intersect_sphere_shadow(point.origin, light_dir_n, obj);
	else if (obj->type == PLANE)
		distance = intersect_plane_shadow(data, light_dir_n, obj);
	else if (obj->type == CYLINDER)
		distance = intersect_cylinder_shadow(point.origin, light_dir_n, obj);
	else if (obj->type == CONE)
		distance = intersect_cone_shadow(point.origin, light_dir_n, obj);
	else
		distance = -1.0f;
	return (distance);
}
