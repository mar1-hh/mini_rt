#include "../minirt.h"

void	render_pixel(t_minirt *data, int x, int y)
{
	t_vec3	ray_direction;
	t_point	point;
	t_vec3	normal;
	int		color;

	ray_direction = generate_rays(data, x, y);
	point = find_closest_inter(data, ray_direction);
	if (point.hit_something)
	{
		normal = calculate_surface_normal(&point);
		normal = apply_surface_effects(&point, normal);
		color = handle_light_shadow(data, &point, normal);
		mlx_put_pixel(data->img, x, y, color);
	}
	else
	{
		color = get_rgba(0, 0, 0, 255);
		mlx_put_pixel(data->img, x, y, color);
	}
}

t_vec3	calculate_surface_normal(t_point *point)
{
	if (point->obj->type == SPHERE)
		return (normalize(sub_vec(point->origin, point->obj->origin)));
	else if (point->obj->type == PLANE)
		return (point->obj->normal);
	else if (point->obj->type == CYLINDER)
		return (get_cylinder_normal(point->origin, point->obj));
	else if (point->obj->type == CONE)
		return (get_cone_normal(point->origin, point->obj));
	return (create_vec(0, 1, 0));
}

t_vec3	apply_surface_effects(t_point *point, t_vec3 normal)
{
	if (point->obj->texture == CHECKER)
		handle_checker(point, point->obj);
	else if (point->obj->texture == BUMP)
		normal = handle_bump(point, point->obj, normal);
	return (normal);
}

uint32_t	get_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

void	rays_setup(t_minirt *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			render_pixel(data, i, j);
			j++;
		}
		i++;
	}
}
