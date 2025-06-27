
#ifndef MINIRT_H
#define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <libft/libft.h>
# include <gnl/get_next_line.h>
# include <mlx.h>

typedef enum    e_types
{
	AMBIENT,
	CAMERA,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
}   t_types;

typedef struct s_object
{
	t_types	type;
	float		x;
	float		y;
	float		z;
	float		diameter;
	unsigned int	colore;
	struct s_object *next;
}   t_object;

typedef struct s_ambiant
{
	float		ratio;
	unsigned int	color;
}   t_ambient;

typedef struct s_camera
{
	float		x;
	float		y;
	float		z;
	float		fov;
}   t_camera;
typedef struct s_light
{
	float		x;
	float		y;
	float		z;
	float		ratio;
	unsigned int	color;
}   t_light;

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

#endif