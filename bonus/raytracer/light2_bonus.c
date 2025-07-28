/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:56:48 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/28 18:11:05 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt_bonus.h"

void	clr_reflection(t_l_s *s_l_data)
{
	s_l_data->color.r += s_l_data->light->color.r / s_l_data->specular_strength
		* s_l_data->spec * s_l_data->light->ratio;
	s_l_data->color.g += s_l_data->light->color.g / s_l_data->specular_strength
		* s_l_data->spec * s_l_data->light->ratio;
	s_l_data->color.b += s_l_data->light->color.b / s_l_data->specular_strength
		* s_l_data->spec * s_l_data->light->ratio;
}

t_vec3	reflect(t_vec3 incident, t_vec3 normal)
{
	return (sub_vec(incident, mul_vec(normal, 2 * dot(incident, normal))));
}

void	process_all_lights(t_minirt *data, t_l_s *s_l_data, t_point *point,
		t_lih *ligh)
{
	while (s_l_data->light)
	{
		s_l_data->light_dir_n = normalize(sub_vec(s_l_data->light->origin,
					point->origin));
		if (!is_shadow(data, *point, s_l_data->light_dir_n,
				s_l_data->light->origin))
		{
			s_l_data->lfar9 = fmax(0.0f, dot(ligh->normal,
						s_l_data->light_dir_n));
			s_l_data->reflect_dir = reflect(mul_vec(s_l_data->light_dir_n, -1),
					ligh->normal);
			s_l_data->view_dir = normalize(sub_vec(data->camera.origin,
						point->origin));
			s_l_data->spec = powf(fmax(dot(s_l_data->view_dir,
							s_l_data->reflect_dir), 0.0f), s_l_data->shininess);
			color_handle_help(data, s_l_data, &ligh->surface_color,
				&s_l_data->light->color);
			clr_reflection(s_l_data);
		}
		s_l_data->light = s_l_data->light->next;
	}
}

void	color_handle_ambient(t_minirt *data, t_l_s *s_l_data, t_color *obj,
		t_color *c_light)
{
	s_l_data->color.r += obj->r * c_light->r / 255.0f * data->ambient.ratio;
	s_l_data->color.g += obj->g * c_light->g / 255.0f * data->ambient.ratio;
	s_l_data->color.b += obj->b * c_light->b / 255.0f * data->ambient.ratio;
}

int	handle_light_shadow(t_minirt *data, t_point *point, t_vec3 normal)
{
	t_l_s	s_l_data;
	t_lih	ligh;

	init_l_s(&s_l_data);
	s_l_data.light = data->light;
	ligh.surface_color = get_surface_color(point);
	ligh.normal = normal;
	process_all_lights(data, &s_l_data, point, &ligh);
	color_handle_ambient(data, &s_l_data, &ligh.surface_color,
		&data->ambient.color);
	clamp_color(&s_l_data.color);
	return (pack_color(s_l_data.color));
}
