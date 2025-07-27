#include "../minirt_bonus.h"

t_vec3	calculate_ray_direction(t_minirt *data, float screen_x, float screen_y)
{
	t_vec3	forward;
	t_vec3	world_up;
	t_vec3	right;
	t_vec3	up;

	forward = normalize(data->camera.normal);
	world_up = create_vec(0, 1, 0);
	right = normalize(cross(forward, world_up));
	up = cross(forward, right);
	return (normalize(add_vec(forward, add_vec(mul_vec(right, screen_x),
					mul_vec(up, screen_y)))));
}
t_vec3	generate_rays(t_minirt *data, int x, int y)
{
	float	aspect_ratio;
	float	fov_rad;
	float	screen_x;
	float	screen_y;

	aspect_ratio = (float)WIDTH / HEIGHT;
	fov_rad = tan(data->camera.fov / 2 * M_PI / 180);
	screen_x = (2 * (x + 0.5) / WIDTH - 1) * aspect_ratio * fov_rad;
	screen_y = (2 * (y + 0.5) / HEIGHT - 1) * fov_rad;
	return (calculate_ray_direction(data, screen_x, screen_y));
}

float	get_object_distance(t_minirt *data, t_vec3 ray_direction,
		t_object *current)
{
	float	distance;

	distance = -1.0f;
	if (current->type == SPHERE)
		distance = intersect_sphere(data, ray_direction, current);
	else if (current->type == PLANE)
		distance = intersect_plane(data, ray_direction, current);
	else if (current->type == CYLINDER)
		distance = intersect_cylinder(data, ray_direction, current);
	else if (current->type == CONE)
		distance = intersect_cone(data, ray_direction, current);
	return (distance);
}
t_point	set_closest_point(t_minirt *data, t_vec3 ray_direction,
		t_object *closest_obj, float closest_distance)
{
	t_point	point;

	if (closest_obj != NULL)
	{
		point.distance = closest_distance;
		point.hit_something = 1;
		point.origin = add_vec(data->camera.origin, mul_vec(ray_direction,
					closest_distance));
		point.obj = closest_obj;
	}
	else
	{
		point.distance = -1.0f;
		point.hit_something = 0;
		point.origin = create_vec(0, 0, 0);
		point.obj = NULL;
	}
	return (point);
}
t_point	find_closest_inter(t_minirt *data, t_vec3 ray_direction)
{
	t_object	*current;
	t_object	*closest_obj;
	float		closest_distance;
	t_point		point;

	current = data->objects;
	closest_obj = NULL;
	closest_distance = MAX_F;
	while (current != NULL)
	{
		point.distance = get_object_distance(data, ray_direction, current);
		if (point.distance > 0.001f && point.distance < closest_distance)
		{
			closest_obj = current;
			closest_distance = point.distance;
		}
		current = current->next;
	}
	return (set_closest_point(data, ray_direction, closest_obj,
			closest_distance));
}
