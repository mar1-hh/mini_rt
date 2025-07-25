/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:08:59 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/25 12:09:00 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

t_vec3	get_cylinder_normal(t_vec3 intersection_point, t_object *cylinder)
{
	t_vec3	axis;
	t_vec3	base_to_point;
	float	projection_length;
	t_vec3	axis_point;

	axis = normalize(cylinder->normal);
	base_to_point = sub_vec(intersection_point, cylinder->origin);
	projection_length = dot(base_to_point, axis);
	axis_point = add_vec(cylinder->origin, mul_vec(axis, projection_length));
	return (normalize(sub_vec(intersection_point, axis_point)));
}

void	calculate_tangent_bitangent(t_vec3 original_normal, t_vec3 *tangent,
		t_vec3 *bitangent)
{
	if (fabs(original_normal.y) < 0.9f)
		*tangent = normalize(cross(original_normal, (t_vec3){0, 1, 0}));
	else
		*tangent = normalize(cross(original_normal, (t_vec3){1, 0, 0}));
	*bitangent = normalize(cross(original_normal, *tangent));
}

t_vec3	get_cone_normal(t_vec3 intersection_point, t_object *cone)
{
	t_vars_3 vars;

	vars.axis = normalize(cone->normal);
	vars.apex = add_vec(cone->origin, mul_vec(vars.axis, cone->height));
	vars.base_to_point = sub_vec(intersection_point, cone->origin);
	vars.height_proj = dot(vars.base_to_point, vars.axis);
	vars.axis_point = add_vec(cone->origin, mul_vec(vars.axis, vars.height_proj));
	vars.radial = normalize(sub_vec(intersection_point, vars.axis_point));
	vars.radius_at_height = cone->diameter / 2.0f * (cone->height - vars.height_proj)
		/ cone->height;
	vars.normal = normalize(add_vec(vars.radial, mul_vec(vars.axis, vars.radius_at_height
					/ cone->height)));
	return (vars.normal);
}
