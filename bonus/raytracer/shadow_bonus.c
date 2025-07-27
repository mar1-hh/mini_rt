#include "../minirt_bonus.h"

int	is_shadow(t_minirt *data, t_point point, t_vec3 light_dir_n,
		t_vec3 origin_light)
{
	float		max_distance;
	float		distance;
	t_object	*obj;

	max_distance = vec_length(sub_vec(origin_light, point.origin));
	obj = data->objects;
	while (obj)
	{
		if (obj != point.obj)
		{
			distance = get_shadow_distance(data, point, light_dir_n, obj);
			if (distance > 0.0f && distance < max_distance)
				return (1);
		}
		obj = obj->next;
	}
	return (0);
}
