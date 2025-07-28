/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:48:49 by msaadaou          #+#    #+#             */
/*   Updated: 2025/07/28 17:59:03 by msaadaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../ft_libft/libft.h"
# include "get_next_line_bonus.h"
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define MAX_F 23234.0f

# define WIDTH 1000
# define HEIGHT 1000

typedef enum e_types
{
	AMBIENT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
}					t_types;

typedef enum e_txture_type
{
	NONE,
	CHECKER,
	BUMP,
}					t_texture;

typedef struct s_vec3
{
	float			x;
	float			y;
	float			z;
}					t_vec3;

typedef struct s_color
{
	float			r;
	float			g;
	float			b;
}					t_color;

typedef struct s_txt
{
	int				tex_x;
	int				tex_y;
}					t_txt;

typedef struct s_object
{
	t_types			type;
	t_vec3			origin;
	t_vec3			normal;
	t_color			color;
	float			diameter;
	float			height;
	t_texture		texture;
	mlx_texture_t	*bump_texture;
	struct s_object	*next;
}					t_object;

typedef struct s_ray
{
	t_vec3			*origine;
	t_vec3			*dir;
}					t_ray;

typedef struct s_ambiant
{
	float			ratio;
	t_color			color;
}					t_ambient;

typedef struct s_camera
{
	t_vec3			origin;
	t_vec3			normal;
	t_vec3			*org;
	float			fov;
}					t_camera;
typedef struct s_light
{
	t_vec3			origin;
	float			ratio;
	t_color			color;
	struct s_light	*next;
}					t_light;

typedef struct s_point
{
	t_vec3			origin;
	float			distance;
	bool			hit_something;
	t_object		*obj;
	t_color			color;
}					t_point;

typedef struct s_minirt
{
	t_object		*objects;
	t_ambient		ambient;
	t_camera		camera;
	t_light			*light;
	mlx_image_t		*img;
	void			*mlx;
	void			*mlx_window;
	char			*addr;
	int				bits_per_pexel;
	int				line_length;
	int				endian;
}					t_minirt;

typedef struct s_cone_vars
{
	t_vec3			oc;
	t_vec3			axis;
	float			radius;
	float			height;
	t_vec3			apex;
	t_vec3			oa;
	float			cos_squared;
	float			vdot_axis;
	float			oa_dot_axis;
	float			a;
	float			b;
	float			c;
}					t_cone_vars;

typedef struct s_l_s
{
	t_light			*light;
	t_vec3			light_dir_n;
	t_color			color;
	float			lfar9;
	float			shininess;
	float			spec;
	float			specular_strength;
	t_vec3			reflect_dir;
	t_vec3			view_dir;
}					t_l_s;

typedef struct s_cyhelp
{
	t_vec3			ray_origin;
	t_vec3			ray_direction;
	t_object		*current;
	t_vec3			axis;
}					t_cyhelp;
typedef struct s_var_1
{
	float			height;
	t_vec3			p1;
	t_vec3			to_p1;
	float			h1;
	t_vec3			p2;
	t_vec3			to_p2;
	float			h2;
}					t_var_1;
typedef struct s_vars_2
{
	t_vec3			oc;
	float			dot_rd_axis;
	float			dot_oc_axis;
	t_vec3			rd_perp;
	t_vec3			oc_perp;
	float			a;
	float			b;
	float			radius;
	float			c;
}					t_var_2;
typedef struct s_vars_3
{
	t_vec3			axis;
	t_vec3			apex;
	t_vec3			base_to_point;
	float			height_proj;
	t_vec3			axis_point;
	t_vec3			radial;
	float			radius_at_height;
	t_vec3			normal;
}					t_vars_3;
typedef struct s_lih
{
	t_vec3			normal;
	t_color			surface_color;
}					t_lih;

float				vec_length(t_vec3 v);
t_vec3				normalize(t_vec3 v);
t_vec3				sub_vec(t_vec3 a, t_vec3 b);
t_vec3				add_vec(t_vec3 a, t_vec3 b);
t_vec3				mul_vec(t_vec3 a, float b);
t_vec3				cross(t_vec3 a, t_vec3 b);
float				dot(t_vec3 a, t_vec3 b);
t_vec3				create_vec(int a, int b, int c);

t_vec3				get_cylinder_normal(t_vec3 intersection_point,
						t_object *cylinder);
t_vec3				get_cone_normal(t_vec3 intersection_point, t_object *cone);
t_vec3				calculate_surface_normal(t_point *point);
t_vec3				apply_surface_effects(t_point *point, t_vec3 normal);
void				calculate_tangent_bitangent(t_vec3 original_normal,
						t_vec3 *tangent, t_vec3 *bitangent);

float				intersect_sphere(t_minirt *data, t_vec3 ray_direction,
						t_object *current);
float				intersect_plane(t_minirt *data, t_vec3 ray_direction,
						t_object *current);
float				intersect_cylinder(t_minirt *data, t_vec3 ray_direction,
						t_object *current);
float				intersect_cone(t_minirt *data, t_vec3 ray_direction,
						t_object *current);

t_vec3				generate_rays(t_minirt *data, int x, int y);
t_point				find_closest_inter(t_minirt *data, t_vec3 ray_direction);
int					handle_light_shadow(t_minirt *data, t_point *point,
						t_vec3 normal);
uint32_t			get_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

float				intersect_sphere_unified(t_vec3 ray_origin,
						t_vec3 ray_direction, t_object *sphere);
float				intersect_cylinder_unified(t_vec3 ray_origin,
						t_vec3 ray_direction, t_object *current);
float				intersect_cone_unified(t_vec3 ray_origin,
						t_vec3 ray_direction, t_object *current);

float				calculate_cylinder_intersection(float a, float b, float c,
						t_cyhelp cyhelp);
float				handle_cylinder_degenerate(float c);
float				check_cylinder_heights(float t1, float t2, t_cyhelp cyhelp);

float				cone_intersect_calc(t_cone_vars *vars, t_vec3 ray_origin,
						t_vec3 ray_direction);
int					check_cone_intersection(float t, t_vec3 ray_origin,
						t_vec3 ray_direction, t_cone_vars *vars);
float				handle_cone_degenerate(t_cone_vars *vars, t_vec3 ray_origin,
						t_vec3 ray_direction);
float				intersect_cone_unified(t_vec3 ray_origin,
						t_vec3 ray_direction, t_object *current);
float				calculate_cone_coefficients(t_cone_vars *vars,
						t_vec3 ray_origin, t_vec3 ray_direction);

void				calculate_texture_coords(t_point *point, t_object *obj,
						float *u, float *v);

t_color				sample_texture(mlx_texture_t *texture, float u, float v);
t_color				get_texture_color(t_point *point, t_object *obj);
void				handle_checker(t_point *point, t_object *obj);
t_vec3				handle_bump(t_point *point, t_object *obj,
						t_vec3 original_normal);

void				get_uv_coordinates(t_point *point, t_object *obj, float *u,
						float *v);

float				check_descriminant(float a, float b, float c);
t_vec3				calculate_ray_direction(t_minirt *data, float screen_x,
						float screen_y);
t_point				initialize_point(void);

void				render_pixel(t_minirt *data, int x, int y);
t_color				get_surface_color(t_point *point);
void				my_mlx_p_pix(unsigned int color, int x, int y,
						t_minirt *data);

void				process_all_lights(t_minirt *data, t_l_s *s_l_data,
						t_point *point, t_lih *ligh);
void				color_handle_help(t_minirt *data, t_l_s *s_l_data,
						t_color *obj, t_color *c_light);
void				clr_reflection(t_l_s *s_l_data);
t_vec3				reflect(t_vec3 incident, t_vec3 normal);
int					is_shadow(t_minirt *data, t_point point, t_vec3 light_dir_n,
						t_vec3 origin_light);
float				get_shadow_distance(t_minirt *data, t_point point,
						t_vec3 light_dir_n, t_object *obj);

void				rays_setup(t_minirt *data);
void				parse_file(char *filename, t_minirt *data);
int					key_code(int code, t_minirt *data);
int					close_window(t_minirt *data);

int					ft_isspace(char str);
void				skip_exept(char **line, char to_skip);
void				skip_space(char **line);
t_vec3				parse_vec3(char **line);
char				*ft_strdup_line(char *line);
void				parse_ambient(char *line, t_ambient *ambient);
void				parse_light(char *line, t_light *light);
void				parse_camera(char *line, t_camera *camera);
int					check_texture_type(char *texture);
void				handling_pump_tex(char **line, t_object *object);
void				parse_plane(char *line, t_object *object);
void				parse_sphere(char *line, t_object *object, t_minirt *data);
void				parse_cylinder(char *line, t_object *object);
void				parse_cone(char *line, t_object *object);
void				parse_light_line(char *line, t_minirt *data);
void				parse_object_line(char *line, t_minirt *data);

void				cleanup_lights(t_light *light);
void				cleanup_objects(t_object *objects);
void				cleanup_data(t_minirt *data);
int					is_dotrt(char *str);
void				init_data(t_minirt *data);

int					is_valide_float(char *str);
double				ft_atof(char *str);

void				init_l_s(t_l_s *data);
void				clamp_color(t_color *color);
int					pack_color(t_color color);

void				calculate_sphere_uv(t_point *point, t_object *obj, float *u,
						float *v);
void				calculate_plane_uv(t_point *point, t_object *obj, float *u,
						float *v);
void				calculate_cylinder_uv(t_point *point, t_object *obj,
						float *u, float *v);
void				calculate_cone_uv(t_point *point, t_object *obj, float *u,
						float *v);

#endif