#include "minirt.h"

void draw_circle(t_minirt *data, int cx, int cy, float radius, int color)
{
    int x;
    int y;

    y = -radius;
    while (y <= radius)
    {
        x = -radius;
        while (x <= radius)
        {
            if (y * y + x * x <= radius * radius)
                my_mlx_p_pix(color, cx + x, cy + y, data);
            x++;
        }
        y++;
    }
}
