#include "../minirt_bonus.h"

static int	get_sphere_checker(t_point *point, t_object *obj, float scale)
{
	t_vec3	hit;
	float	u;
	float	v;
	int		check_x;
	int		check_y;

	hit = normalize(sub_vec(point->origin, obj->origin));
	u = 0.5 + atan2(hit.z, hit.x) / (2 * M_PI);
	v = 0.5 - asin(hit.y) / M_PI;
	check_x = (int)floor(u * scale);
	check_y = (int)floor(v * scale);
	return (check_x + check_y);
}

static int	get_plane_checker(t_point *point, float scale)
{
	int	check_x;
	int	check_y;
	int	check_z;

	check_x = (int)floor(point->origin.x * scale);
	check_y = (int)floor(point->origin.y * scale);
	check_z = (int)floor(point->origin.z * scale);
	return (check_x + check_y + check_z);
}

static int	get_cylinder_checker(t_point *point, t_object *obj, float scale)
{
	t_vec3	local_hit;
	float	theta;
	float	u;
	float	v;
	int		check_x;

	local_hit = sub_vec(point->origin, obj->origin);
	theta = atan2(local_hit.z, local_hit.x);
	u = (theta + M_PI) / (2 * M_PI);
	v = local_hit.y;
	check_x = (int)floor(u * scale);
	return (check_x + (int)floor(v * scale));
}

static int	get_cone_checker(t_point *point, t_object *obj, float scale)
{
	t_vec3	local_hit;
	float	theta;
	float	u;
	float	v;
	int		check_x;

	local_hit = sub_vec(point->origin, obj->origin);
	theta = atan2(local_hit.z, local_hit.x);
	u = (theta + M_PI) / (2 * M_PI);
	v = local_hit.y;
	check_x = (int)floor(u * scale);
	return (check_x + (int)floor(v * scale));
}

void	handle_checker(t_point *point, t_object *obj)
{
	float	scale;
	int		checker;

	scale = 10.0f;
	checker = 0;
	if (obj->type == SPHERE)
		checker = get_sphere_checker(point, obj, scale);
	else if (obj->type == PLANE)
		checker = get_plane_checker(point, scale);
	else if (obj->type == CYLINDER)
		checker = get_cylinder_checker(point, obj, scale);
	else if (obj->type == CONE)
		checker = get_cone_checker(point, obj, scale);
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
