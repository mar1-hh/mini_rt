#include "../minirt.h"


static void	get_texture_coordinates(t_object *obj, float u, float v, int *tex_x, int *tex_y)
{
    *tex_x = (int)(u * obj->bump_texture->width);
    *tex_y = (int)(v * obj->bump_texture->height);
    *tex_x = fmax(0, fmin(*tex_x, obj->bump_texture->width - 1));
    *tex_y = fmax(0, fmin(*tex_y, obj->bump_texture->height - 1));
}

static void	get_rgb_values(t_object *obj, int tex_x, int tex_y, float *r, float *g, float *b)
{
    uint8_t	*pixels;
    int		index;

    pixels = obj->bump_texture->pixels;
    index = (tex_y * obj->bump_texture->width + tex_x) * 4;
    *r = (pixels[index] / 255.0f - 0.5f) * 2.0f;
    *g = (pixels[index + 1] / 255.0f - 0.5f) * 2.0f;
    *b = (pixels[index + 2] / 255.0f - 0.5f) * 2.0f;
}

static void	calculate_tangent_bitangent(t_vec3 original_normal, t_vec3 *tangent, t_vec3 *bitangent)
{
    if (fabs(original_normal.y) < 0.9f)
        *tangent = normalize(cross(original_normal, (t_vec3){0, 1, 0}));
    else
        *tangent = normalize(cross(original_normal, (t_vec3){1, 0, 0}));
    *bitangent = normalize(cross(original_normal, *tangent));
}

t_vec3	handle_bump(t_point *point, t_object *obj, t_vec3 original_normal)
{
    float	u;
    float	v;
    int		tex_x;
    int		tex_y;
    float	r;
    float	g;
    float	b;
    t_vec3	tangent;
    t_vec3	bitangent;
    float	bump_strength;
    t_vec3	bump_normal;

    if (!obj->bump_texture)
        return (original_normal);
    get_uv_coordinates(point, obj, &u, &v);
    get_texture_coordinates(obj, u, v, &tex_x, &tex_y);
    get_rgb_values(obj, tex_x, tex_y, &r, &g, &b);
    calculate_tangent_bitangent(original_normal, &tangent, &bitangent);
    bump_strength = 0.03f;
    bump_normal = add_vec(add_vec(mul_vec(tangent, r * bump_strength),
            mul_vec(bitangent, g * bump_strength)),
        mul_vec(original_normal, 1.0f + b * bump_strength * 0.5f));
    return (normalize(bump_normal));
}
