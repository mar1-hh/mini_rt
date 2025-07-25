#include "../minirt.h"

float	handle_cylinder_degenerate(float c)
{
	if (c >= 0)
		return (-1.0f);
	return (-1.0f);
}

float	check_cylinder_heights(float t1, float t2, t_cyhelp cyhelp)
{
	float	height;
	t_vec3	p1;
	t_vec3	to_p1;
	float	h1;
	t_vec3	p2;
	t_vec3	to_p2;
	float	h2;

	height = cyhelp.current->height;
	if (t1 > 0.001f)
	{
		p1 = add_vec(cyhelp.ray_origin, mul_vec(cyhelp.ray_direction, t1));
		to_p1 = sub_vec(p1, cyhelp.current->origin);
		h1 = dot(to_p1, cyhelp.axis);
		if (h1 >= 0 && h1 <= height)
			return (t1);
	}
	if (t2 > 0.001f)
	{
		p2 = add_vec(cyhelp.ray_origin, mul_vec(cyhelp.ray_direction, t2));
		to_p2 = sub_vec(p2, cyhelp.current->origin);
		h2 = dot(to_p2, cyhelp.axis);
		if (h2 >= 0 && h2 <= height)
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
	t_vec3		oc;
	float		dot_rd_axis;
	float		dot_oc_axis;
	t_vec3		rd_perp;
	t_vec3		oc_perp;
	float		a;
	float		b;
	float		radius;
	float		c;

	oc = sub_vec(ray_origin, current->origin);
	cyhelp.axis = normalize(current->normal);
	cyhelp.current = current;
	cyhelp.ray_direction = ray_direction;
	cyhelp.ray_origin = ray_origin;
	dot_rd_axis = dot(ray_direction, cyhelp.axis);
	dot_oc_axis = dot(oc, cyhelp.axis);
	rd_perp = sub_vec(ray_direction, mul_vec(cyhelp.axis, dot_rd_axis));
	oc_perp = sub_vec(oc, mul_vec(cyhelp.axis, dot_oc_axis));
	a = dot(rd_perp, rd_perp);
	b = 2.0f * dot(oc_perp, rd_perp);
	radius = current->diameter / 2.0f;
	c = dot(oc_perp, oc_perp) - radius * radius;
	return (calculate_cylinder_intersection(a, b, c, cyhelp));
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
