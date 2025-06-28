#include "../minirt.h"

t_vector    *add_vector(t_vector *a, t_vector *b)
{
	t_vector	*new;

	new = malloc(sizeof(t_vector));
	new->x = a->x + b->x;
	new->y = a->y + b->y;
	new->z = a->z + b->z;
	return (new);
}
