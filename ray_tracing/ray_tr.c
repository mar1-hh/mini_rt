#include "../minirt.h"

float	scale_x_y(float num)
{
	float	rate;
	float	res;

	rate = num * 100 / 799;
	res = 2 * rate / 100;
	return (res - 1);
}

void    start_rt(t_minirt *data)
{
    int i;
    int j;
    float   fov_rad;
    float   mt;
    float   plan_width;
    float   plan_height;
    float   aspect_ratio;
    float   new_x;
    float   new_y;

    fov_rad = data->camera.fov * (M_PI / 180);
    mt = tan(fov_rad / 2);
    aspect_ratio = WIDTH / HEIGHT;
    plan_height = mt * 2;
    plan_width = aspect_ratio * plan_height;
    i = 0;
    while (i < HEIGHT)
    {
        j = 0;
        new_y = scale_x_y(i);
        while (j < WIDTH)
        {
            new_x = scale_x_y(j);
            j++;
        }
        i++;
    }
}