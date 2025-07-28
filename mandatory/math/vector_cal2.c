/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_cal2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:06:33 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/27 19:13:52 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

t_vec3	cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y
		- a.y * b.x});
}

float	dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

float	check_descriminant(float a, float b, float c)
{
	float	discriminant;
	float	sqrt_discriminant;
	float	t1;
	float	t2;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0f);
	sqrt_discriminant = sqrtf(discriminant);
	t1 = (-b - sqrt_discriminant) / (2 * a);
	t2 = (-b + sqrt_discriminant) / (2 * a);
	if (t1 > 0.001f)
		return (t1);
	if (t2 > 0.001f)
		return (t2);
	return (-1.0f);
}

t_vec3	create_vec(int a, int b, int c)
{
	t_vec3	vec;

	vec.x = a;
	vec.y = b;
	vec.z = c;
	return (vec);
}
