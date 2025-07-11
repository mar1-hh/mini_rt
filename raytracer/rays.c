

#include "../minirt.h"
#include <math.h>
#include <stdio.h>

float vec_length(t_vec3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
t_vec3 normalize(t_vec3 v) {
	float len = vec_length(v);
	if (len < 1e-6f)
		return v;
	return (t_vec3){v.x / len, v.y / len, v.z / len};
}
t_vec3 sub_vec(t_vec3 a, t_vec3 b) {
	return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}
t_vec3 add_vec(t_vec3 a, t_vec3 b) {
	return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}
t_vec3 mul_vec(t_vec3 a, float b) {
	return (t_vec3){a.x * b, a.y * b, a.z * b};
}
t_vec3 cross(t_vec3 a, t_vec3 b) {
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

float distance(t_vec3 a, t_vec3 b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float dz = b.z - a.z;
	return sqrtf(dx*dx + dy*dy + dz*dz);
}


t_vec3 generate_rays(t_minirt *data, int x, int y)
{
	float aspect_ratio;
	float fov_rad;
	float screen_x;
	float screen_y;
	t_vec3 ray_direction;
	aspect_ratio = (float)WIDTH / HEIGHT;
	fov_rad = tan(data->camera.fov / 2 * M_PI / 180);

	screen_x = (2 * (x + 0.5) / WIDTH - 1) * aspect_ratio * fov_rad;
	screen_y = (2 * (y + 0.5) / HEIGHT - 1) * fov_rad; 

	t_vec3 forward = normalize(data->camera.normal);
	t_vec3 world_up = {0, 1, 0};
	t_vec3 right = normalize(cross(forward, world_up));
	t_vec3 up = cross(forward, right);
	ray_direction = normalize(
		add_vec(forward, 
		add_vec(mul_vec(right, screen_x),
		mul_vec(up, screen_y))));
	return ray_direction;
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

float intersect_sphere(t_minirt *data, t_vec3 ray_direction, t_object *current)
{

	t_vec3 L = sub_vec(data->camera.origin, current->origin);
	float radius = current->diameter/2.0f;
	float a = 1.0f;
	float b = 2.0f * dot(ray_direction,L);
	float c = dot(L, L) - radius * radius;
	return check_descriminant(a, b, c);

}
float intersect_plane(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	float denom = dot(current->normal, ray_direction);

	if (fabs(denom) < 1e-6)
		return -1;

	t_vec3 L = sub_vec(current->origin, data->camera.origin);
	float t = dot(current->normal, L) / denom;
	if (t < 0)
		return -1.0f;
	return t;
}
float intersect_cylinder(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	t_vec3 L = sub_vec(data->camera.origin, current->origin);
	float a = ray_direction.x * ray_direction.x + ray_direction.z * ray_direction.z;
	float b = 2 * (ray_direction.x * L.x + ray_direction.z * L.z);
	float c = L.x * L.x + L.z * L.z - (current->diameter / 2.0f) * (current->diameter / 2.0f);
	float t = check_descriminant(a, b, c);

	if (t > 0.001f)
	{
		t_vec3 intersection_point = add_vec(data->camera.origin, mul_vec(ray_direction, t));
		if (intersection_point.y >= current->origin.y && intersection_point.y <= current->origin.y + current->height)
			return t;
	}
	return -1.0f;
}

float intersect_cone_shadow(t_vec3 *light, t_vec3 ray_direction, t_object *current)
{
	t_vec3 co = sub_vec(*light, current->origin);
	t_vec3 axis = normalize(current->normal);
	float radius = current->diameter / 2.0f;
	float height = current->height;
	float k = radius / height;
	float k_s = k * k;
	t_vec3 v = ray_direction;

	float a = dot(v, v) - (1 + k_s) * dot(v, axis) * dot(v, axis);
	float b = 2 * (dot(v, co) - (1 + k_s) * dot(v, axis) * dot(co, axis));
	float c = dot(co, co) - (1 + k_s) * dot(co, axis) * dot(co, axis);
	
	float t = check_descriminant(a, b, c);
	if (t > 0.001f)
	{
		t_vec3 p = add_vec(*light, mul_vec(ray_direction, t));
		float h = dot(sub_vec(p, current->origin), axis);
		if (h >= 0 && h <= height)
			return t;
	}
	return -1.0f;
}
float intersect_cone(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	t_vec3 co = sub_vec(data->camera.origin, current->origin);
	t_vec3 axis = normalize(current->normal);
	float radius = current->diameter / 2.0f;
	float height = current->height;
	float k = radius / height;
	float k_s = k * k;
	t_vec3 v = ray_direction;

	float a = dot(v, v) - (1 + k_s) * dot(v, axis) * dot(v, axis);
	float b = 2 * (dot(v, co) - (1 + k_s) * dot(v, axis) * dot(co, axis));
	float c = dot(co, co) - (1 + k_s) * dot(co, axis) * dot(co, axis);
	
	float t = check_descriminant(a, b, c);
	if (t > 0.001f)
	{
		t_vec3 p = add_vec(data->camera.origin, mul_vec(ray_direction, t));
		float h = dot(sub_vec(p, current->origin), axis);
		if (h >= 0 && h <= height)
			return t;
	}
	return -1.0f;
}


t_point find_closest_inter(t_minirt *data, t_vec3 ray_direction)
{
	t_object *current = data->objects;
	t_object *closest_obj = NULL;
	float closest_distance = MAX_F;
	t_point point;

	point.distance = -1.0f;
	point.hit_something = 0;
	point.origin = (t_vec3){0, 0, 0};
	point.obj = NULL;
	while (current != NULL)
	{
		float distance = -1.0f;
		if (current->type == SPHERE)
			distance = intersect_sphere(data, ray_direction, current);
		else if (current->type == PLANE)
			distance = intersect_plane(data, ray_direction, current);
		else if (current->type == CYLINDER)
			distance = intersect_cylinder(data, ray_direction, current);
		else if (current->type == CONE)
			distance = intersect_cone(data, ray_direction, current);
		if (distance > 0.001f && distance < closest_distance) {
			closest_obj = current;
			closest_distance = distance;
		}
		current = current->next;
	}
	if (closest_obj != NULL)
	{
		point.distance = closest_distance;
		point.hit_something = 1;
		point.origin = add_vec(data->camera.origin, mul_vec(ray_direction, closest_distance));
		point.obj = closest_obj;
	}
	return point;
}

float intersect_cylinder_shadow(t_vec3 ray_origin, t_vec3 ray_direction, t_object *current)
{
	t_vec3 L = sub_vec(ray_origin, current->origin);
	float a = ray_direction.x * ray_direction.x + ray_direction.z * ray_direction.z;
	float b = 2 * (ray_direction.x * L.x + ray_direction.z * L.z);
	float c = L.x * L.x + L.z * L.z - (current->diameter / 2.0f) * (current->diameter / 2.0f);
	float t = check_descriminant(a, b, c);

	if (t > 0.001f)
	{
		t_vec3 intersection_point = add_vec(ray_origin, mul_vec(ray_direction, t));
		if (intersection_point.y >= current->origin.y && intersection_point.y <= current->origin.y + current->height)
			return t;
	}
	return -1.0f;
}

float intersect_plane_shadow(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
	float denom = dot(current->normal, ray_direction);

	if (fabs(denom) < 1e-6)
		return -1;

	t_vec3 L = sub_vec(current->origin, data->camera.origin);
	float t = dot(current->normal, L) / denom;
	if (t < 0)
		return -1.0f;
	return t;
}
float intersect_sphere_shadow(t_vec3 ray_origin, t_vec3 ray_direction, t_object *sphere)
{
	t_vec3 oc = sub_vec(ray_origin, sphere->origin);
	float radius = sphere->diameter / 2.0f;

	float a = dot(ray_direction, ray_direction);
	float b = 2.0f * dot(oc, ray_direction);
	float c = dot(oc, oc) - radius * radius;

	return check_descriminant(a, b, c);
}


int is_shadow(t_minirt *data, t_point point, t_vec3 light_dir_n, t_vec3 origin_light)
{
	float   max_dstance;
	float   dstance;
	t_object    *obj;

	max_dstance = vec_length(sub_vec(origin_light, point.origin));
	obj = data->objects;
	while (obj)
	{
		if (obj != point.obj)
		{
			if (obj->type == SPHERE)
				dstance = intersect_sphere_shadow(point.origin, light_dir_n, obj);
			else if (obj->type == CYLINDER)
				dstance = intersect_cylinder_shadow(point.origin, light_dir_n, obj);
			else if (obj->type == PLANE)
			{
				obj = obj->next;
				continue ;
			}
			else if (obj->type == CONE)
				dstance = intersect_cone_shadow(&origin_light, light_dir_n, obj);
			
			if (dstance > 0.0f && dstance < max_dstance)
				return (1);
		}
		obj = obj->next;
	}
	return (0);
}

t_vec3 reflect(t_vec3 incident, t_vec3 normal)
{
	return sub_vec(incident, mul_vec(normal, 2 * dot(incident, normal)));
}

void init_l_s(t_l_s *data)
{
	data->shininess = 700;
	data->specular_strength = 0.5;
	data->color.r = 0;
	data->color.g = 0;
	data->color.b = 0;
}

void	color_handle_help(t_minirt *data, t_l_s *s_l_data, t_color *obj, t_color *c_light)
{
	s_l_data->color.r += obj->r * c_light->r / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
	s_l_data->color.g += obj->g * c_light->g / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
	s_l_data->color.b += obj->b * c_light->b / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
}

int handle_light_shadow(t_minirt *data, t_point *point, t_vec3 normal)
{
	t_l_s	s_l_data;

	init_l_s(&s_l_data);
	s_l_data.light = data->light;
	while (s_l_data.light)
	{
		s_l_data.light_dir_n = normalize(sub_vec(s_l_data.light->origin, point->origin));
		if (!is_shadow(data, *point, s_l_data.light_dir_n, s_l_data.light->origin))
		{
			s_l_data.lfar9 = fmax(0.0f, dot(normal, s_l_data.light_dir_n));
			s_l_data.reflect_dir = reflect(mul_vec(s_l_data.light_dir_n, -1), normal);
			s_l_data.view_dir = normalize(sub_vec(data->camera.origin, point->origin));
			s_l_data.spec = powf(fmax(dot(s_l_data.view_dir, s_l_data.reflect_dir), 0.0f), s_l_data.shininess);
			if (point->obj->texture == CHECKER)
			{
				// color_handle_help(data, &s_l_data, )
				s_l_data.color.r += point->r * s_l_data.light->R / 255.0f * s_l_data.lfar9 * s_l_data.light->ratio;
				s_l_data.color.g += point->g * s_l_data.light->G / 255.0f * s_l_data.lfar9 * s_l_data.light->ratio;
				s_l_data.color.b += point->b * s_l_data.light->B / 255.0f * s_l_data.lfar9 * s_l_data.light->ratio;
			}
			else
			{
				s_l_data.color.r += point->obj->R * s_l_data.light->R / 255.0f * s_l_data.lfar9 * s_l_data.light->ratio;
				s_l_data.color.g += point->obj->G * s_l_data.light->G / 255.0f * s_l_data.lfar9 * s_l_data.light->ratio;
				s_l_data.color.b += point->obj->B * s_l_data.light->B / 255.0f * s_l_data.lfar9 * s_l_data.light->ratio;
			}
			s_l_data.color.r += s_l_data.light->R / s_l_data.specular_strength * s_l_data.spec * s_l_data.light->ratio;
			s_l_data.color.g += s_l_data.light->G / s_l_data.specular_strength * s_l_data.spec * s_l_data.light->ratio;
			s_l_data.color.b += s_l_data.light->B / s_l_data.specular_strength * s_l_data.spec * s_l_data.light->ratio;
		}
		s_l_data.light = s_l_data.light->next;
	}
	if (point->obj->texture == CHECKER)
	{
		s_l_data.color.r += point->r * data->ambient.R / 255.0f * data->ambient.ratio;
		s_l_data.color.g += point->g * data->ambient.G / 255.0f * data->ambient.ratio;  
		s_l_data.color.b += point->b * data->ambient.B / 255.0f * data->ambient.ratio;
	}
	else
	{
		s_l_data.color.r += point->obj->R * data->ambient.R / 255.0f * data->ambient.ratio;
		s_l_data.color.g += point->obj->G * data->ambient.G / 255.0f * data->ambient.ratio;
		s_l_data.color.b += point->obj->B * data->ambient.B / 255.0f * data->ambient.ratio;
	}
	s_l_data.color.r = fmin(s_l_data.color.r, 255.0f);
	s_l_data.color.g = fmin(s_l_data.color.g, 255.0f);
	s_l_data.color.b = fmin(s_l_data.color.b, 255.0f);
	return ((int)s_l_data.color.r << 16) | ((int)s_l_data.color.g << 8) | (int)s_l_data.color.b;
}

void handle_checker(t_point *point, t_object *obj)
{
	int check_x;
	int check_y;
	int check_z;
	float scale = 10;
	int checker; 
	if (obj->type == SPHERE) {
		t_vec3 hit = normalize(sub_vec(point->origin, obj->origin));

		float u = 0.5 + atan2(hit.z, hit.x) / (2 * M_PI);
		float v = 0.5 - asin(hit.y) / M_PI;

		check_x = (int)floor(u * scale);
		check_y = (int)floor(v * scale);
		checker = check_x + check_y;
	}

	else if (obj->type == PLANE) {
		check_x = (int)floor(point->origin.x * scale);
		check_y = (int)floor(point->origin.y * scale);
		check_z = (int)floor(point->origin.z * scale);
		checker = check_x + check_y + check_z;
	}
	else if (obj->type == CYLINDER)
	{
		t_vec3 local_hit = sub_vec(point->origin, obj->origin);
		float theta = atan2(local_hit.z, local_hit.x);
		float u = (theta + M_PI) / (2 * M_PI);

		float v = local_hit.y;

		check_x = (int)floor(u * scale);
		check_y = (int)floor(v * scale);

		checker = check_x + check_y;
	}
	else if (obj->type == CONE)
	{
		t_vec3 local_hit = sub_vec(point->origin, obj->origin);
		float theta = atan2(local_hit.z, local_hit.x);
		float u = (theta + M_PI) / (2 * M_PI);
		
		float v = local_hit.y;
		
		check_x = (int)floor(u * scale);
		check_y = (int)floor(v * scale);
		checker = check_x + check_y;
	}
	if (checker % 2 == 0)
	{
		point->r = 255;
		point->g = 255;
		point->b = 255;
	}
	else
	{
		point->r = 0;
		point->g = 0;
		point->b = 0;
	}
}

void rays_setup(t_minirt *data)
{
	int i, j;
	t_point point;
	t_vec3 normal;
	float intensity;
	int color;

	i = 0;
	while (i < WIDTH) 
	{
		j = 0;
		while (j < HEIGHT) 
		{
			t_vec3 ray_direction = generate_rays(data, i, j);
			point = find_closest_inter(data, ray_direction);
			if (point.hit_something)
			{
				if (point.obj->type == SPHERE)
					normal = normalize(sub_vec(point.origin, point.obj->origin));
				else if (point.obj->type == PLANE)
					normal = point.obj->normal;
				else if (point.obj->type == CYLINDER)
				{
					t_vec3 axis_point = {point.obj->origin.x, point.origin.y, point.obj->origin.z};
					normal = normalize(sub_vec(point.origin, axis_point));
				}
				else if (point.obj->type == CONE)
				{
					t_vec3 axis = normalize(point.obj->normal);
					t_vec3 apex_to_point = sub_vec(point.origin, point.obj->origin);
					float h = dot(apex_to_point, axis);
					t_vec3 projected = sub_vec(apex_to_point, mul_vec(axis, h));
					float radius = point.obj->diameter / 2.0f;
					float height = point.obj->height;
					float k = radius / height;
					normal = normalize(sub_vec(apex_to_point, mul_vec(axis, k * k * h)));
				}
				if (point.obj->texture == CHECKER)
					handle_checker(&point, point.obj);
				
				color = handle_light_shadow(data, &point, normal);
				my_mlx_p_pix(color, i, j, data);
			}
			j++;
		}
		i++;
	}
}
