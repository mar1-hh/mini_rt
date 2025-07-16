#include "../minirt.h"

t_vec3 cross(t_vec3 a, t_vec3 b)
{
	return (t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}
float dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

float distance(t_vec3 a, t_vec3 b)
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float dz = b.z - a.z;
	return sqrtf(dx*dx + dy*dy + dz*dz);
}
float check_descriminant(float a, float b, float c)
{
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return -1.0f;
	float sqrt_discriminant = sqrtf(discriminant);
	float t1 = (-b - sqrt_discriminant) / (2 * a);
	float t2 = (-b + sqrt_discriminant) / (2 * a);
	if (t1 > 0.001f)
		return t1;
	if (t2 > 0.001f)
		return t2;
	return -1.0f;
}
t_vec3 create_vec(int a, int b, int c)
{
    t_vec3 vec;

    vec.x = a;
    vec.y = b;
    vec.z = c;
    return vec;
}

