/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_bump_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:50:08 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/28 17:50:09 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

static void	get_texture_coordinates(t_object *obj, float u, float v, t_txt *txt)
{
	txt->tex_x = (int)(u * obj->bump_texture->width);
	txt->tex_y = (int)(v * obj->bump_texture->height);
	txt->tex_x = fmax(0, fmin(txt->tex_x, obj->bump_texture->width - 1));
	txt->tex_y = fmax(0, fmin(txt->tex_y, obj->bump_texture->height - 1));
}

static void	get_rgb_values(t_object *obj, t_txt txt, t_color *clr)
{
	uint8_t	*pixels;
	int		index;

	pixels = obj->bump_texture->pixels;
	index = (txt.tex_y * obj->bump_texture->width + txt.tex_x) * 4;
	clr->r = (pixels[index] / 255.0f - 0.5f) * 2.0f;
	clr->g = (pixels[index + 1] / 255.0f - 0.5f) * 2.0f;
	clr->b = (pixels[index + 2] / 255.0f - 0.5f) * 2.0f;
}

static t_vec3	calculate_bump_displacement(t_object *obj, float u, float v,
		t_vec3 original_normal)
{
	t_txt	txt;
	t_color	clr;

	get_texture_coordinates(obj, u, v, &txt);
	get_rgb_values(obj, txt, &clr);
	return ((t_vec3){clr.r, clr.g, clr.b});
}

static t_vec3	apply_bump_to_normal(t_vec3 displacement,
		t_vec3 original_normal)
{
	t_vec3	tangent;
	t_vec3	bitangent;
	float	bump_strength;
	t_vec3	bump_normal;

	calculate_tangent_bitangent(original_normal, &tangent, &bitangent);
	bump_strength = 0.03f;
	bump_normal = add_vec(add_vec(mul_vec(tangent, displacement.x
					* bump_strength), mul_vec(bitangent, displacement.y
					* bump_strength)), mul_vec(original_normal, 1.0f
				+ displacement.z * bump_strength * 0.5f));
	return (normalize(bump_normal));
}

t_vec3	handle_bump(t_point *point, t_object *obj, t_vec3 original_normal)
{
	float	u;
	float	v;
	t_vec3	displacement;

	if (!obj->bump_texture)
		return (original_normal);
	get_uv_coordinates(point, obj, &u, &v);
	displacement = calculate_bump_displacement(obj, u, v, original_normal);
	return (apply_bump_to_normal(displacement, original_normal));
}
