
#ifndef MINIRT_H
#define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "get_next_line/get_next_line.h"
# include <math.h>
# include "ft_libft/libft.h"
# include <stdbool.h>
// # include "minilibx-linux/mlx.h"
# include "MLX42/include/MLX42/MLX42.h"
#define MAX_F (1.0f / 0.0f)

# define WIDTH 1000
# define HEIGHT 1000


typedef enum    e_types
{
	AMBIENT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
}   t_types;

typedef enum e_txture_type
{
	NONE,
	CHECKER,
	BUMP,
}   t_texture;

typedef struct s_vec3
{
	float x;
	float y;
	float z;
}   t_vec3;

typedef struct s_color
{
	float r;
	float g;
	float b;
}   t_color;

// typedef struct s_texdata {
//     void    *img_ptr;
//     char    *data;
//     int     width;
//     int     height;
//     int     bpp;
//     int     size_line;
//     int     endian;
// }   t_texdata;


typedef struct s_object
{
	t_types	type;
	t_vec3	origin;
	t_vec3	normal;
	t_color	color;
	float	diameter;
	float	height;
	t_texture	texture;
	mlx_texture_t *bump_texture;
	struct s_object *next;
}   t_object;

// typedef struct	s_vector
// {
// 	float	x;
// 	float	y;
// 	float	z;
// }	t_vector;

typedef struct	s_ray
{
	t_vec3	*origine;
	t_vec3	*dir;
}	t_ray;


typedef struct s_ambiant
{
	float		ratio;
	t_color		color;
}   t_ambient;

typedef struct s_camera
{
	t_vec3	origin;
	t_vec3	normal;
	t_vec3	*org;
	float		fov;
}   t_camera;
typedef struct s_light
{
	t_vec3	origin;
	float		ratio;
	t_color	color;
	struct s_light *next;
}   t_light;

typedef struct s_point
{
	t_vec3 origin;
	float distance;
	bool hit_something;
	t_object *obj;
	t_color	color;
	// int color;
}	t_point;

typedef struct s_minirt
{
	t_object	*objects;
	t_ambient	ambient;
	t_camera	camera;
	t_light		*light;
	mlx_image_t	*img;
	void	*mlx;
	void	*mlx_window;
	char	*addr;
	int		bits_per_pexel;
	int		line_length;
	int		endian;
}   t_minirt;

typedef struct s_cone_vars
{
    t_vec3	oc;
    t_vec3	axis;
    float	radius;
    float	height;
    t_vec3	apex;
    t_vec3	oa;
    float	cos_squared;
    float	vdot_axis;
    float	oa_dot_axis;
    float	a;
    float	b;
    float	c;
}	t_cone_vars;

typedef struct s_l_s
{
	t_light *light;
    t_vec3  light_dir_n;
    t_color  color;
    float   lfar9;
    float shininess;
    float   spec;
    float   specular_strength;
    t_vec3  reflect_dir;
	t_vec3  view_dir;
}   t_l_s;
// Vector operations
float vec_length(t_vec3 v);
t_vec3 normalize(t_vec3 v);
t_vec3 sub_vec(t_vec3 a, t_vec3 b);
t_vec3 add_vec(t_vec3 a, t_vec3 b);
t_vec3 mul_vec(t_vec3 a, float b);
t_vec3 cross(t_vec3 a, t_vec3 b);
float dot(t_vec3 a, t_vec3 b);
float distance(t_vec3 a, t_vec3 b);
t_vec3 create_vec(int a, int b, int c);
// Normal calculation functions
t_vec3 get_cylinder_normal(t_vec3 intersection_point, t_object *cylinder);
t_vec3 get_cone_normal(t_vec3 intersection_point, t_object *cone);
t_vec3 calculate_surface_normal(t_point *point);
t_vec3 apply_surface_effects(t_point *point, t_vec3 normal);
// Intersection functions - Main interface
float intersect_sphere(t_minirt *data, t_vec3 ray_direction, t_object *current);
float intersect_plane(t_minirt *data, t_vec3 ray_direction, t_object *current);
float intersect_cylinder(t_minirt *data, t_vec3 ray_direction, t_object *current);
float intersect_cone(t_minirt *data, t_vec3 ray_direction, t_object *current);
//rays
t_vec3	generate_rays(t_minirt *data, int x, int y);
t_point	find_closest_inter(t_minirt *data, t_vec3 ray_direction);
int	handle_light_shadow(t_minirt *data, t_point *point, t_vec3 normal);
uint32_t get_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

// Intersection functions - Unified (for shadows and main)
float intersect_sphere_unified(t_vec3 ray_origin, t_vec3 ray_direction, t_object *sphere);
float intersect_cylinder_unified(t_vec3 ray_origin, t_vec3 ray_direction, t_object *current);
float intersect_cone_unified(t_vec3 ray_origin, t_vec3 ray_direction, t_object *current);

// Cylinder intersection helpers
float calculate_cylinder_intersection(float a, float b, float c, t_vec3 ray_origin, 
                                    t_vec3 ray_direction, t_object *current, t_vec3 axis);
float handle_cylinder_degenerate(float c, t_vec3 ray_origin, t_vec3 ray_direction, 
                                t_object *current, t_vec3 axis);
float validate_cylinder_height(float t, t_vec3 ray_origin, t_vec3 ray_direction, 
                              t_object *current, t_vec3 axis);
float check_cylinder_heights(float t1, float t2, t_vec3 ray_origin, t_vec3 ray_direction, 
                            t_object *current, t_vec3 axis);

// Cone intersection helpers
void setup_cone_vars(t_cone_vars *vars, t_vec3 ray_origin, t_object *current);
float calculate_cone_coefficients(t_cone_vars *vars, t_vec3 ray_origin, t_vec3 ray_direction);
float calculate_discriminant(t_cone_vars *vars, t_vec3 ray_direction);
int validate_cone_intersection(float t, t_vec3 ray_origin, t_vec3 ray_direction, t_cone_vars *vars);

// Texture coordinate calculation
void calculate_sphere_coords(t_point *point, t_object *obj, float *u, float *v);
void calculate_plane_coords(t_point *point, t_object *obj, float *u, float *v);
void calculate_cylinder_coords(t_point *point, t_object *obj, float *u, float *v);
void calculate_cone_coords(t_point *point, t_object *obj, float *u, float *v);
void calculate_texture_coords(t_point *point, t_object *obj, float *u, float *v);

// Texture sampling
t_color sample_texture(mlx_texture_t *texture, float u, float v);
t_color get_texture_color(t_point *point, t_object *obj);
void handle_checker(t_point *point, t_object *obj);
t_vec3 handle_bump(t_point *point, t_object *obj, t_vec3 original_normal);
void	calculate_sphere_coords(t_point *point, t_object *obj, float *u, float *v);
void	calculate_plane_coords(t_point *point, t_object *obj, float *u, float *v);

// Utility functions
float check_descriminant(float a, float b, float c);
t_vec3 calculate_ray_direction(t_minirt *data, float screen_x, float screen_y);
t_point initialize_point(void);

// Rendering functions
void render_pixel(t_minirt *data, int x, int y);
t_vec3 calculate_surface_normal(t_point *point);
t_vec3 apply_surface_effects(t_point *point, t_vec3 normal);
t_color get_surface_color(t_point *point);
void draw_circle(t_minirt *data, int cx, int cy, float radius, int color);
void my_mlx_p_pix(unsigned int color, int x, int y, t_minirt *data);

// light
void process_all_lights(t_minirt *data, t_l_s *s_l_data, t_point *point, 
	t_vec3 normal, t_color surface_color);
void color_handle_help(t_minirt *data, t_l_s *s_l_data, t_color *obj, t_color *c_light);
void clr_reflection(t_l_s *s_l_data);
t_vec3 reflect(t_vec3 incident, t_vec3 normal);
int	is_shadow(t_minirt *data, t_point point, t_vec3 light_dir_n,
	t_vec3 origin_light);
	float get_shadow_distance(t_minirt *data, t_point point, t_vec3 light_dir_n, t_object *obj);
// Main functions
void rays_setup(t_minirt *data);
void parse_file(char *filename, t_minirt *data);
int key_code(int code, t_minirt *data);
int close_window(t_minirt *data);

// Parsing utilities
int is_valide_float(char *str);
double ft_atof(char *str);
//color
void	init_l_s(t_l_s *data);
void	clamp_color(t_color *color);
int	pack_color(t_color color);
//bump
void	calculate_sphere_uv(t_point *point, t_object *obj, float *u, float *v);
void	calculate_plane_uv(t_point *point, t_object *obj, float *u, float *v);
void	calculate_cylinder_uv(t_point *point, t_object *obj, float *u, float *v);
void	calculate_cone_uv(t_point *point, t_object *obj, float *u, float *v);
void	get_uv_coordinates(t_point *point, t_object *obj, float *u, float *v);
//render 
t_vec3 get_cone_normal(t_vec3 intersection_point, t_object *cone);
// float vec_length(t_vec3 v);
// t_vec3 normalize(t_vec3 v);
// t_vec3 sub_vec(t_vec3 a, t_vec3 b);
// t_vec3 add_vec(t_vec3 a, t_vec3 b);
// t_vec3 mul_vec(t_vec3 a, float b);
// t_vec3 cross(t_vec3 a, t_vec3 b);
// float dot(t_vec3 a, t_vec3 b);
// float distance(t_vec3 a, t_vec3 b);
// float check_descriminant(float a, float b, float c);
// float intersect_sphere(t_minirt *data, t_vec3 ray_direction, t_object *current);
// float intersect_plane(t_minirt *data, t_vec3 ray_direction, t_object *current);
// float intersect_cylinder(t_minirt *data, t_vec3 ray_direction, t_object *current);
// float intersect_cone(t_minirt *data, t_vec3 ray_direction, t_object *current);
// float	intersect_sphere_unified(t_vec3 ray_origin, t_vec3 ray_direction,
// 	t_object *sphere);
// float	intersect_cylinder_unified(t_vec3 ray_origin, t_vec3 ray_direction,
// 	t_object *current);
// float	calculate_cylinder_intersection(float a, float b, float c,
// 	t_vec3 ray_origin, t_vec3 ray_direction, t_object *current,
// 	t_vec3 axis);
// float	handle_cylinder_degenerate(float c, t_vec3 ray_origin,
// 	t_vec3 ray_direction, t_object *current, t_vec3 axis);
// float	validate_cylinder_height(float t, t_vec3 ray_origin,
// 	t_vec3 ray_direction, t_object *current, t_vec3 axis);
// float	check_cylinder_heights(float t1, float t2, t_vec3 ray_origin,
// 	t_vec3 ray_direction, t_object *current, t_vec3 axis);
// float	intersect_cone_unified(t_vec3 ray_origin, t_vec3 ray_direction,
// 	t_object *current);
// void	setup_cone_vars(t_cone_vars *vars, t_vec3 ray_origin,
// 	t_object *current);
// float	calculate_cone_coefficients(t_cone_vars *vars, t_vec3 ray_origin,
// 	t_vec3 ray_direction);
// float	calculate_discriminant(t_cone_vars *vars, t_vec3 ray_direction);
// int		validate_cone_intersection(float t, t_vec3 ray_origin,
// t_vec3 ray_direction, t_cone_vars *vars);
// t_vec3	calculate_ray_direction(t_minirt *data, float screen_x, float screen_y);
// t_point	initialize_point(void);
// void	render_pixel(t_minirt *data, int x, int y);
// t_vec3	calculate_surface_normal(t_point *point);
// void	apply_surface_effects(t_point *point, t_vec3 normal);
// t_color	get_surface_color(t_point *point);
// void calculate_sphere_coords(t_point *point, t_object *obj, float *u, float *v);
// void calculate_plane_coords(t_point *point, t_object *obj, float *u, float *v);
// t_color get_texture_color(t_point *point, t_object *obj);
// void calculate_cylinder_coords(t_point *point, t_object *obj, float *u, float *v);
// void calculate_cone_coords(t_point *point, t_object *obj, float *u, float *v);
// t_color sample_texture(mlx_texture_t *texture, float u, float v);
// void	calculate_texture_coords(t_point *point, t_object *obj, float *u, float *v);
// void	draw_circle(t_minirt *data, int cx, int cy, float radius, int color);
// t_vec3 create_vec(int a, int b, int c);
// void    my_mlx_p_pix(unsigned int color, int x, int y, t_minirt *data);
// int	is_valide_float(char *str);
// double	ft_atof(char *str);
// void parse_file(char *filename, t_minirt *data);
// void rays_setup(t_minirt *data);
// int key_code(int code, t_minirt *data);
// int close_window(t_minirt *data);
#endif