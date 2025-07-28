/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:17:40 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/27 19:19:29 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

float	intersect_sphere(t_minirt *data, t_vec3 ray_direction,
		t_object *current)
{
	return (intersect_sphere_unified(data->camera.origin, ray_direction,
			current));
}

float	intersect_plane(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	float	denom;
	t_vec3	l;
	float	t;

	denom = dot(current->normal, ray_direction);
	if (fabs(denom) < 1e-6)
		return (-1);
	l = sub_vec(current->origin, data->camera.origin);
	t = dot(current->normal, l) / denom;
	if (t < 0)
		return (-1.0f);
	return (t);
}

float	intersect_cylinder(t_minirt *data, t_vec3 ray_direction,
		t_object *current)
{
	return (intersect_cylinder_unified(data->camera.origin, ray_direction,
			current));
}

float	intersect_cone(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	return (intersect_cone_unified(data->camera.origin, ray_direction,
			current));
}
