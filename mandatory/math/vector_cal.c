#include "../minirt.h"

float vec_length(t_vec3 v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
t_vec3 normalize(t_vec3 v)
{
	float len = vec_length(v);
	if (len < 1e-6f)
		return v;
	return (t_vec3){v.x / len, v.y / len, v.z / len};
}
t_vec3 sub_vec(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}
t_vec3 add_vec(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}
t_vec3 mul_vec(t_vec3 a, float b)
{
	return (t_vec3){a.x * b, a.y * b, a.z * b};
}