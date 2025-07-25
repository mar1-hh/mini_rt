/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:08:15 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/25 12:08:16 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

float	handle_cone_degenerate(t_cone_vars *vars, t_vec3 ray_origin,
		t_vec3 ray_direction)
{
	float	t;
	t_vec3	p;
	t_vec3	ap;
	float	proj;

	if (fabs(vars->b) < 1e-6f)
		return (-1.0f);
	t = -vars->c / vars->b;
	if (t > 0.001f)
	{
		p = add_vec(ray_origin, mul_vec(ray_direction, t));
		ap = sub_vec(p, vars->apex);
		proj = -dot(ap, vars->axis);
		if (proj >= 0 && proj <= vars->height)
			return (t);
	}
	return (-1.0f);
}
