#include "../minirt.h"

float	handle_cylinder_degenerate(float c)
{
	if (c >= 0)
		return (-1.0f);
	return (-1.0f);
}

float	check_cylinder_heights(float t1, float t2, t_cyhelp cyhelp)
{
	t_var_1 vars;

	vars.height = cyhelp.current->height;
	if (t1 > 0.001f)
	{
		vars.p1 = add_vec(cyhelp.ray_origin, mul_vec(cyhelp.ray_direction, t1));
		vars.to_p1 = sub_vec(vars.p1, cyhelp.current->origin);
		vars.h1 = dot(vars.to_p1, cyhelp.axis);
		if (vars.h1 >= 0 && vars.h1 <= vars.height)
			return (t1);
	}
	if (t2 > 0.001f)
	{
		vars.p2 = add_vec(cyhelp.ray_origin, mul_vec(cyhelp.ray_direction, t2));
		vars.to_p2 = sub_vec(vars.p2, cyhelp.current->origin);
		vars.h2 = dot(vars.to_p2, cyhelp.axis);
		if (vars.h2 >= 0 && vars.h2 <= vars.height)
			return (t2);
	}
	return (-1.0f);
}

float	intersect_sphere_unified(t_vec3 ray_origin, t_vec3 ray_direction,
		t_object *sphere)
{
	t_vec3	oc;
	float	radius;
	float	a;
	float	b;
	float	c;

	oc = sub_vec(ray_origin, sphere->origin);
	radius = sphere->diameter / 2.0f;
	a = dot(ray_direction, ray_direction);
	b = 2.0f * dot(oc, ray_direction);
	c = dot(oc, oc) - radius * radius;
	return (check_descriminant(a, b, c));
}

float	intersect_cylinder_unified(t_vec3 ray_origin, t_vec3 ray_direction,
		t_object *current)
{
	t_cyhelp	cyhelp;
	t_var_2     vars;

	vars.oc = sub_vec(ray_origin, current->origin);
	cyhelp.axis = normalize(current->normal);
	cyhelp.current = current;
	cyhelp.ray_direction = ray_direction;
	cyhelp.ray_origin = ray_origin;
	vars.dot_rd_axis = dot(ray_direction, cyhelp.axis);
	vars.dot_oc_axis = dot(vars.oc, cyhelp.axis);
	vars.rd_perp = sub_vec(ray_direction, mul_vec(cyhelp.axis, vars.dot_rd_axis));
	vars.oc_perp = sub_vec(vars.oc, mul_vec(cyhelp.axis, vars.dot_oc_axis));
	vars.a = dot(vars.rd_perp, vars.rd_perp);
	vars.b = 2.0f * dot(vars.oc_perp, vars.rd_perp);
	vars.radius = current->diameter / 2.0f;
	vars.c = dot(vars.oc_perp, vars.oc_perp) - vars.radius * vars.radius;
	return (calculate_cylinder_intersection(vars.a, vars.b, vars.c, cyhelp));
}

float	calculate_cylinder_intersection(float a, float b, float c,
		t_cyhelp cyhelp)
{
	float	discriminant;
	float	sqrt_discriminant;
	float	t1;
	float	t2;

	if (a < 1e-6f)
		return (handle_cylinder_degenerate(c));
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0f);
	sqrt_discriminant = sqrt(discriminant);
	t1 = (-b - sqrt_discriminant) / (2.0f * a);
	t2 = (-b + sqrt_discriminant) / (2.0f * a);
	return (check_cylinder_heights(t1, t2, cyhelp));
}
