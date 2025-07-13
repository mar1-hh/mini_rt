
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
# include <mlx.h>
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

typedef struct s_texdata {
    void    *img_ptr;
    char    *data;
    int     width;
    int     height;
    int     bpp;
    int     size_line;
    int     endian;
}   t_texdata;


typedef struct s_object
{
	t_types	type;
	t_vec3	origin;
	t_vec3	normal;
	t_color	color;
	float	diameter;
	float	height;
	t_texture	texture;
	t_texdata data;
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
	void	*img;
	void	*mlx;
	void	*mlx_window;
	char	*addr;
	int		bits_per_pexel;
	int		line_length;
	int		endian;
}   t_minirt;

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

void	draw_circle(t_minirt *data, int cx, int cy, float radius, int color);
void    my_mlx_p_pix(unsigned int color, int x, int y, t_minirt *data);
int	is_valide_float(char *str);
double	ft_atof(char *str);
void parse_file(char *filename, t_minirt *data);
void rays_setup(t_minirt *data);
int key_code(int code, t_minirt *data);
int close_window(t_minirt *data);
#endif