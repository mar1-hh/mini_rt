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

t_vector	*normalize(t_vector dir)
{
	t_vector	*new;
	float		len;

	len = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	new->x = dir.x / len;
	new->y = dir.y / len;
	new->z = dir.z / len;
	return (new);
}
