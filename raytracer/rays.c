#include "../minirt.h"

t_vec3	calculate_ray_direction(t_minirt *data, float screen_x, float screen_y)
{
	t_vec3		forward;
	t_vec3		world_up;
	t_vec3		right;
	t_vec3		up;
	t_vec3		ray_direction;

	forward = normalize(data->camera.normal);
	world_up = create_vec(0, 1, 0);
	right = normalize(cross(forward, world_up));
	up = cross(forward, right);
	ray_direction = normalize(add_vec(forward, add_vec(mul_vec(right, screen_x),
				mul_vec(up, screen_y))));
	return (ray_direction);
}
t_vec3	generate_rays(t_minirt *data, int x, int y)
{
    float		aspect_ratio;
    float		fov_rad;
    float		screen_x;
    float		screen_y;
    t_vec3		ray_direction;

    aspect_ratio = (float)WIDTH / HEIGHT;
    fov_rad = tan(data->camera.fov / 2 * M_PI / 180);
    screen_x = (2 * (x + 0.5) / WIDTH - 1) * aspect_ratio * fov_rad;
    screen_y = (2 * (y + 0.5) / HEIGHT - 1) * fov_rad;
    ray_direction = calculate_ray_direction(data, screen_x, screen_y);
    return (ray_direction);
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
		point.origin = add_vec(data->camera.origin,
				mul_vec(ray_direction, closest_distance));
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
	float distance;

    current = data->objects;
    closest_obj = NULL;
    closest_distance = MAX_F;
    point.distance = -1.0f;
    point.hit_something = 0;
    point.origin = create_vec(0, 0, 0);
    point.obj = NULL;
    while (current != NULL)
    {
        distance = get_object_distance(data, ray_direction, current);
        if (distance > 0.001f && distance < closest_distance)
        {
            closest_obj = current;
            closest_distance = distance;
        }
        current = current->next;
    }
    return (set_closest_point(data, ray_direction, closest_obj,
            closest_distance));
}


void handle_checker(t_point *point, t_object *obj)
{
	int check_x;
	int check_y;
	int check_z;
	float scale = 10;
	int checker; 
	if (obj->type == SPHERE) {
		t_vec3 hit = normalize(sub_vec(point->origin, obj->origin));

		float u = 0.5 + atan2(hit.z, hit.x) / (2 * M_PI);
		float v = 0.5 - asin(hit.y) / M_PI;

		check_x = (int)floor(u * scale);
		check_y = (int)floor(v * scale);
		checker = check_x + check_y;
	}

	else if (obj->type == PLANE) {
		check_x = (int)floor(point->origin.x * scale);
		check_y = (int)floor(point->origin.y * scale);
		check_z = (int)floor(point->origin.z * scale);
		checker = check_x + check_y + check_z;
	}
	else if (obj->type == CYLINDER)
	{
		t_vec3 local_hit = sub_vec(point->origin, obj->origin);
		float theta = atan2(local_hit.z, local_hit.x);
		float u = (theta + M_PI) / (2 * M_PI);

		float v = local_hit.y;

		check_x = (int)floor(u * scale);
		check_y = (int)floor(v * scale);

		checker = check_x + check_y;
	}
	else if (obj->type == CONE)
	{
		t_vec3 local_hit = sub_vec(point->origin, obj->origin);
		float theta = atan2(local_hit.z, local_hit.x);
		float u = (theta + M_PI) / (2 * M_PI);
		
		float v = local_hit.y;
		
		check_x = (int)floor(u * scale);
		check_y = (int)floor(v * scale);
		checker = check_x + check_y;
	}
	if (checker % 2 == 0)
	{
		point->color.r = 255;
		point->color.g = 255;
		point->color.b = 255;
	}
	else
	{
		point->color.r = 0;
		point->color.g = 0;
		point->color.b = 0;
	}
}
t_vec3 get_cylinder_normal(t_vec3 intersection_point, t_object *cylinder)
{
    t_vec3 axis;
    t_vec3 base_to_point;
    float projection_length;
    t_vec3 axis_point;
    t_vec3 normal;

    axis = normalize(cylinder->normal);
    base_to_point = sub_vec(intersection_point, cylinder->origin);
    projection_length = dot(base_to_point, axis);
    axis_point = add_vec(cylinder->origin, mul_vec(axis, projection_length));
    normal = normalize(sub_vec(intersection_point, axis_point));
    
    return normal;
}

