
#ifndef MINIRT_H
#define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "get_next_line/get_next_line.h"
# include <math.h>
# include "ft_libft/libft.h"
# include <stdbool.h>
// # include <libft/libft.h>
// # include <gnl/get_next_line.h>
# include <mlx.h>
#define MAX_F (1.0f / 0.0f)

# define HEIGHT 1920
# define WIDTH 1080

typedef enum    e_types
{
	AMBIENT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
}   t_types;

typedef struct s_vec3
{
	float x;
	float y;
	float z;
}   t_vec3;

typedef struct s_object
{
	t_types	type;
	t_vec3	origin;
	float		diameter;
	int R;
	int G;
	int B;
	t_vec3	normal;
	float height;
	struct s_object *next;
}   t_object;

typedef struct	s_vector
{
	float	x;
	float	y;
	float	z;
}	t_vector;

typedef struct	s_ray
{
	t_vector	*origine;
	t_vector	*dir;
}	t_ray;


typedef struct s_ambiant
{
	float		ratio;
	int R;
	int G;
	int B;
}   t_ambient;

typedef struct s_camera
{
	t_vec3	origin;
	t_vec3	normal;
	t_vector	*org;
	float		fov;
}   t_camera;
typedef struct s_light
{
	t_vec3	origin;
	float		ratio;
	int R;
	int G;
	int B;
	}   t_light;

typedef struct s_point
{
	t_vec3 origin;
	float distance;
	bool hit_something;
	t_object *obj;
	// int color;
}	t_point;
typedef struct s_minirt
{
	t_object	*objects;
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	void	*img;
	void	*mlx;
	void	*mlx_window;
	char	*addr;
	int		bits_per_pexel;
	int		line_length;
	int		endian;
}   t_minirt;

void	draw_circle(t_minirt *data, int cx, int cy, float radius, int color);
void    my_mlx_p_pix(unsigned int color, int x, int y, t_minirt *data);
int	is_valide_float(char *str);
double	ft_atof(char *str);
void parse_file(char *filename, t_minirt *data);
void rays_setup(t_minirt *data);

#endif