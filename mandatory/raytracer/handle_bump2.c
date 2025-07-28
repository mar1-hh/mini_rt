/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_bump2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:08:29 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/27 19:15:41 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

void	calculate_sphere_uv(t_point *point, t_object *obj, float *u, float *v)
{
	t_vec3	hit;

	hit = normalize(sub_vec(point->origin, obj->origin));
	*u = 0.5 + atan2(hit.z, hit.x) / (2 * M_PI);
	*v = 0.5 - asin(hit.y) / M_PI;
	*u = fmax(0.0f, fmin(1.0f, *u));
	*v = fmax(0.0f, fmin(1.0f, *v));
}

void	calculate_plane_uv(t_point *point, t_object *obj, float *u, float *v)
{
	t_vec3	plane_normal;

	plane_normal = normalize(obj->normal);
	if (fabs(plane_normal.y) > 0.9f)
	{
		*u = fmod(point->origin.x * 0.5, 1.0);
		*v = fmod(point->origin.z * 0.5, 1.0);
	}
	else if (fabs(plane_normal.x) > 0.9f)
	{
		*u = fmod(point->origin.z * 0.5, 1.0);
		*v = fmod(point->origin.y * 0.5, 1.0);
	}
	else
	{
		*u = fmod(point->origin.x * 0.5, 1.0);
		*v = fmod(point->origin.y * 0.5, 1.0);
	}
}

void	calculate_cylinder_uv(t_point *point, t_object *obj, float *u, float *v)
{
	t_vec3	local_hit;
	float	theta;

	local_hit = sub_vec(point->origin, obj->origin);
	theta = atan2(local_hit.z, local_hit.x);
	*u = (theta + M_PI) / (2 * M_PI);
	*v = local_hit.y * 0.1;
	*v = fmod(*v, 1.0);
	if (*v < 0)
		*v += 1.0;
}

void	calculate_cone_uv(t_point *point, t_object *obj, float *u, float *v)
{
	t_vec3	axis;
	t_vec3	apex;
	t_vec3	local_hit;
	float	theta;
	float	dist_from_apex;

	axis = normalize(obj->normal);
	apex = add_vec(obj->origin, mul_vec(axis, obj->height));
	local_hit = sub_vec(point->origin, apex);
	theta = atan2(local_hit.z, local_hit.x);
	*u = (theta + M_PI) / (2 * M_PI);
	dist_from_apex = vec_length(local_hit);
	*v = dist_from_apex / (obj->height * 1.41421f);
	*u = fmax(0.0f, fmin(1.0f, *u));
	*v = fmax(0.0f, fmin(1.0f, *v));
}

void	get_uv_coordinates(t_point *point, t_object *obj, float *u, float *v)
{
	if (obj->type == SPHERE)
		calculate_sphere_uv(point, obj, u, v);
	else if (obj->type == PLANE)
	{
		calculate_plane_uv(point, obj, u, v);
		if (*u < 0)
			*u += 1.0;
		if (*v < 0)
			*v += 1.0;
	}
	else if (obj->type == CYLINDER)
		calculate_cylinder_uv(point, obj, u, v);
	else if (obj->type == CONE)
		calculate_cone_uv(point, obj, u, v);
}
