 #include "../minirt.h"

int	check_cone_intersection(float t, t_vec3 ray_origin, t_vec3 ray_direction,
		t_cone_vars *vars)
{
	t_vec3	p;
	t_vec3	ap;
	float	proj;

	if (t > 0.001f)
	{
		p = add_vec(ray_origin, mul_vec(ray_direction, t));
		ap = sub_vec(p, vars->apex);
		proj = -dot(ap, vars->axis);
		if (proj >= 0 && proj <= vars->height)
			return (1);
	}
	return (0);
}

float	cone_intersect_calc(t_cone_vars *vars, t_vec3 ray_origin,
		t_vec3 ray_direction)
{
	float	discriminant;
	float	sqrt_discriminant;
	float	t1;
	float	t2;

	if (fabs(vars->a) < 1e-6f)
		return (handle_cone_degenerate(vars, ray_origin, ray_direction));
	discriminant = vars->b * vars->b - 4 * vars->a * vars->c;
	if (discriminant < 0)
		return (-1.0f);
	sqrt_discriminant = sqrt(discriminant);
	t1 = (-vars->b - sqrt_discriminant) / (2.0f * vars->a);
	t2 = (-vars->b + sqrt_discriminant) / (2.0f * vars->a);
	if (check_cone_intersection(t1, ray_origin, ray_direction, vars))
		return (t1);
	if (check_cone_intersection(t2, ray_origin, ray_direction, vars))
		return (t2);
	return (-1.0f);
}
float	intersect_cone_unified(t_vec3 ray_origin, t_vec3 ray_direction,
		t_object *current)
{
	t_cone_vars	vars;

	vars.oc = sub_vec(ray_origin, current->origin);
	vars.axis = normalize(current->normal);
	vars.radius = current->diameter / 2.0f;
	vars.height = current->height;
	vars.apex = add_vec(current->origin, mul_vec(vars.axis, vars.height));
	calculate_cone_coefficients(&vars, ray_origin, ray_direction);
	return (cone_intersect_calc(&vars, ray_origin, ray_direction));
}
