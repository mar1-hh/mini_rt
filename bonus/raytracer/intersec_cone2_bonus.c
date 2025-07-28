/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersec_cone2_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:55:41 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/28 18:02:49 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

float	calculate_discriminant(t_cone_vars *vars, t_vec3 ray_direction)
{
	float	k;

	k = vars->radius / vars->height;
	k = k * k;
	vars->a = dot(ray_direction, ray_direction) - (1.0f + k) * vars->vdot_axis
		* vars->vdot_axis;
	vars->b = 2.0f * (dot(ray_direction, vars->oa) - (1.0f + k)
			* vars->vdot_axis * vars->oa_dot_axis);
	vars->c = dot(vars->oa, vars->oa) - (1.0f + k) * vars->oa_dot_axis
		* vars->oa_dot_axis;
	return (vars->b * vars->b - 4 * vars->a * vars->c);
}

float	calculate_cone_coefficients(t_cone_vars *vars, t_vec3 ray_origin,
		t_vec3 ray_direction)
{
	vars->oa = sub_vec(ray_origin, vars->apex);
	vars->vdot_axis = dot(ray_direction, vars->axis);
	vars->oa_dot_axis = dot(vars->oa, vars->axis);
	return (calculate_discriminant(vars, ray_direction));
}
