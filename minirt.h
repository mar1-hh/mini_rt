
#ifndef MINIRT_H
#define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <mlx.h>

typedef enum    e_types
{
	sp
}   t_types;

typedef struct s_minirt
{
	t_types type;
	float	x;
	float	y;
	float	z;
	float	diameter;
	unsigned int	colore;
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

#endif