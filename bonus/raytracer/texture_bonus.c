#include "../minirt_bonus.h"

void	calculate_texture_coords(t_point *point, t_object *obj, float *u,
		float *v)
{
	get_uv_coordinates(point, obj, u, v);
}

t_color	sample_texture(mlx_texture_t *texture, float u, float v)
{
	int		tex_x;
	int		tex_y;
	uint8_t	*pixels;
	int		index;
	t_color	texture_color;

	tex_x = (int)(u * texture->width);
	tex_y = (int)(v * texture->height);
	tex_x = fmax(0, fmin(tex_x, texture->width - 1));
	tex_y = fmax(0, fmin(tex_y, texture->height - 1));
	pixels = texture->pixels;
	index = (tex_y * texture->width + tex_x) * 4;
	texture_color.r = pixels[index];
	texture_color.g = pixels[index + 1];
	texture_color.b = pixels[index + 2];
	return (texture_color);
}
t_color	get_texture_color(t_point *point, t_object *obj)
{
	float	u;
	float	v;

	if (!obj->bump_texture)
		return (obj->color);
	calculate_texture_coords(point, obj, &u, &v);
	return (sample_texture(obj->bump_texture, u, v));
}
