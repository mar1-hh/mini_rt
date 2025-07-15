

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
    t_vec3 oc = sub_vec(data->camera.origin, current->origin);
    t_vec3 axis = normalize(current->normal);
    
    float dot_rd_axis = dot(ray_direction, axis);
    float dot_oc_axis = dot(oc, axis);
    
    t_vec3 rd_perp = sub_vec(ray_direction, mul_vec(axis, dot_rd_axis));
    t_vec3 oc_perp = sub_vec(oc, mul_vec(axis, dot_oc_axis));
    
    float a = dot(rd_perp, rd_perp);
    float b = 2.0f * dot(oc_perp, rd_perp);
    float radius = current->diameter / 2.0f;
    float c = dot(oc_perp, oc_perp) - radius * radius;
    
    // Fix: Check for degenerate case (ray parallel to cylinder axis)
    if (a < 1e-6f) {
        // Ray is parallel to cylinder axis
        if (c > 0) return -1.0f; // Ray misses cylinder
        // Ray is inside cylinder, check height bounds only
        float t = -dot_oc_axis / dot_rd_axis;
        if (t > 0.001f) {
            t_vec3 intersection_point = add_vec(data->camera.origin, mul_vec(ray_direction, t));
            t_vec3 point_to_base = sub_vec(intersection_point, current->origin);
            float height_projection = dot(point_to_base, axis);
            if (height_projection >= 0.0f && height_projection <= current->height)
                return t;
        }
        return -1.0f;
    }
    
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;
    
    float sqrt_discriminant = sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);
    
    // Fix: Check both intersections properly
    float t = -1.0f;
    
    // Check t1 first (closer intersection)
    if (t1 > 0.001f) {
        t_vec3 intersection_point = add_vec(data->camera.origin, mul_vec(ray_direction, t1));
        t_vec3 point_to_base = sub_vec(intersection_point, current->origin);
        float height_projection = dot(point_to_base, axis);
        if (height_projection >= 0.0f && height_projection <= current->height)
            return t1;
    }
    
    // Check t2 (farther intersection)
    if (t2 > 0.001f) {
        t_vec3 intersection_point = add_vec(data->camera.origin, mul_vec(ray_direction, t2));
        t_vec3 point_to_base = sub_vec(intersection_point, current->origin);
        float height_projection = dot(point_to_base, axis);
        if (height_projection >= 0.0f && height_projection <= current->height)
            return t2;
    }
    
    return -1.0f;
}

float intersect_cone_shadow(t_vec3 ray_origin, t_vec3 ray_direction, t_object *current)
{
    t_vec3 oc = sub_vec(ray_origin, current->origin);
    t_vec3 axis = normalize(current->normal);
    float radius = current->diameter / 2.0f;
    float height = current->height;
    
    // Cone with apex at (origin + height * axis)
    t_vec3 apex = add_vec(current->origin, mul_vec(axis, height));
    t_vec3 oa = sub_vec(ray_origin, apex);
    
    // Cone angle calculations
    float cos_squared = (height * height) / (radius * radius + height * height);
    
    float vdot_axis = dot(ray_direction, axis);
    float oa_dot_axis = dot(oa, axis);
    
    float a = dot(ray_direction, ray_direction) - cos_squared * vdot_axis * vdot_axis;
    float b = 2.0f * (dot(ray_direction, oa) - cos_squared * vdot_axis * oa_dot_axis);
    float c = dot(oa, oa) - cos_squared * oa_dot_axis * oa_dot_axis;
    
    // Handle degenerate case
    if (fabs(a) < 1e-6f) {
        if (fabs(b) < 1e-6f) return -1.0f;
        float t = -c / b;
        if (t > 0.001f) {
            t_vec3 p = add_vec(ray_origin, mul_vec(ray_direction, t));
            t_vec3 ap = sub_vec(p, apex);
            float proj = dot(ap, mul_vec(axis, -1));
            if (proj >= 0 && proj <= height)
                return t;
        }
        return -1.0f;
    }
    
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;
    
    float sqrt_discriminant = sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);
    
    // Check both intersections
    if (t1 > 0.001f) {
        t_vec3 p = add_vec(ray_origin, mul_vec(ray_direction, t1));
        t_vec3 ap = sub_vec(p, apex);
        float proj = dot(ap, mul_vec(axis, -1));
        if (proj >= 0 && proj <= height)
            return t1;
    }
    
    if (t2 > 0.001f) {
        t_vec3 p = add_vec(ray_origin, mul_vec(ray_direction, t2));
        t_vec3 ap = sub_vec(p, apex);
        float proj = dot(ap, mul_vec(axis, -1));
        if (proj >= 0 && proj <= height)
            return t2;
    }
    
    return -1.0f;
}
float intersect_cone(t_minirt *data, t_vec3 ray_direction, t_object *current)
{
    t_vec3 oc = sub_vec(data->camera.origin, current->origin);
    t_vec3 axis = normalize(current->normal);
    float radius = current->diameter / 2.0f;
    float height = current->height;
    
    // Cone with apex at the TOP (origin + height * axis)
    t_vec3 apex = add_vec(current->origin, mul_vec(axis, height));
    t_vec3 oa = sub_vec(data->camera.origin, apex);
    
    // Use proper cone angle calculation
    float tan_squared = (radius * radius) / (height * height);
    
    float vdot_axis = dot(ray_direction, axis);
    float oa_dot_axis = dot(oa, axis);
    
    // Corrected quadratic equation for cone
    float a = dot(ray_direction, ray_direction) - (1 + tan_squared) * vdot_axis * vdot_axis;
    float b = 2.0f * (dot(ray_direction, oa) - (1 + tan_squared) * vdot_axis * oa_dot_axis);
    float c = dot(oa, oa) - (1 + tan_squared) * oa_dot_axis * oa_dot_axis;
    
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;
    
    float sqrt_discriminant = sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);
    
    // Check t1 first
    if (t1 > 0.001f) {
        t_vec3 p = add_vec(data->camera.origin, mul_vec(ray_direction, t1));
        t_vec3 ap = sub_vec(p, apex);
        float proj = dot(ap, axis);  // Project along axis from apex
        if (proj >= -height && proj <= 0)  // Apex is at top, base below
            return t1;
    }
    
    // Check t2
    if (t2 > 0.001f) {
        t_vec3 p = add_vec(data->camera.origin, mul_vec(ray_direction, t2));
        t_vec3 ap = sub_vec(p, apex);
        float proj = dot(ap, axis);  // Project along axis from apex
        if (proj >= -height && proj <= 0)  // Apex is at top, base below
            return t2;
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
// float intersect_cylinder_shadow(t_vec3 ray_origin, t_vec3 ray_direction, t_object *current)
// {
//     t_vec3 L = sub_vec(ray_origin, current->origin);
//     float a = ray_direction.x * ray_direction.x + ray_direction.z * ray_direction.z;
//     float b = 2 * (ray_direction.x * L.x + ray_direction.z * L.z);
//     float c = L.x * L.x + L.z * L.z - (current->diameter / 2.0f) * (current->diameter / 2.0f);
//     float t = check_descriminant(a, b, c);

//     if (t > 0.001f)
//     {
//         t_vec3 intersection_point = add_vec(ray_origin, mul_vec(ray_direction, t));
//         if (intersection_point.y >= current->origin.y && intersection_point.y <= current->origin.y + current->height)
//             return t;
//     }
//     return -1.0f;
// }
float intersect_cylinder_shadow(t_vec3 ray_origin, t_vec3 ray_direction, t_object *current) 
{
    t_vec3 oc = sub_vec(ray_origin, current->origin);
    t_vec3 axis = normalize(current->normal);
    
    float dot_rd_axis = dot(ray_direction, axis);
    float dot_oc_axis = dot(oc, axis);
    
    t_vec3 rd_perp = sub_vec(ray_direction, mul_vec(axis, dot_rd_axis));
    t_vec3 oc_perp = sub_vec(oc, mul_vec(axis, dot_oc_axis));
    
    float a = dot(rd_perp, rd_perp);
    float b = 2.0f * dot(oc_perp, rd_perp);
    float radius = current->diameter / 2.0f;
    float c = dot(oc_perp, oc_perp) - radius * radius;
    
    // Fix: Same degenerate case handling
    if (a < 1e-6f) {
        if (c > 0) return -1.0f;
        float t = -dot_oc_axis / dot_rd_axis;
        if (t > 0.001f) {
            t_vec3 intersection_point = add_vec(ray_origin, mul_vec(ray_direction, t));
            t_vec3 point_to_base = sub_vec(intersection_point, current->origin);
            float height_projection = dot(point_to_base, axis);
            if (height_projection >= 0.0f && height_projection <= current->height)
                return t;
        }
        return -1.0f;
    }
    
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;
    
    float sqrt_discriminant = sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);
    
    // Check both intersections
    if (t1 > 0.001f) {
        t_vec3 intersection_point = add_vec(ray_origin, mul_vec(ray_direction, t1));
        t_vec3 point_to_base = sub_vec(intersection_point, current->origin);
        float height_projection = dot(point_to_base, axis);
        if (height_projection >= 0.0f && height_projection <= current->height)
            return t1;
    }
    
    if (t2 > 0.001f) {
        t_vec3 intersection_point = add_vec(ray_origin, mul_vec(ray_direction, t2));
        t_vec3 point_to_base = sub_vec(intersection_point, current->origin);
        float height_projection = dot(point_to_base, axis);
        if (height_projection >= 0.0f && height_projection <= current->height)
            return t2;
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
				dstance = intersect_cone_shadow(point.origin, light_dir_n, obj);
			
			if (dstance > 0.0f && dstance < max_dstance)
				return (1);
		}
		obj = obj->next;
	}
	return (0);
}
t_color get_texture_color(t_point *point, t_object *obj)
{
    if (!obj->bump_texture)
        return obj->color;  // Return base color if no texture
    
    // Calculate texture coordinates (same as bump mapping)
    float u, v;
    
    if (obj->type == SPHERE) 
    {
        t_vec3 hit = normalize(sub_vec(point->origin, obj->origin));
        u = 0.5 + atan2(hit.z, hit.x) / (2 * M_PI);
        v = 0.5 - asin(hit.y) / M_PI;
        u = fmax(0.0f, fmin(1.0f, u));
        v = fmax(0.0f, fmin(1.0f, v));
    }
    else if (obj->type == PLANE) 
    {
        // FIXED: Use same orientation logic as handle_bump
        t_vec3 plane_normal = normalize(obj->normal);
        
        if (fabs(plane_normal.y) > 0.9f) 
        {
            // Horizontal plane (floor/ceiling) - use X and Z
            u = fmod(point->origin.x * 0.5, 1.0);
            v = fmod(point->origin.z * 0.5, 1.0);
        }
        else if (fabs(plane_normal.x) > 0.9f) 
        {
            // Vertical plane facing X - use Y and Z  
            u = fmod(point->origin.z * 0.5, 1.0);
            v = fmod(point->origin.y * 0.5, 1.0);
        }
        else 
        {
            // Vertical plane facing Z - use X and Y
            u = fmod(point->origin.x * 0.5, 1.0);
            v = fmod(point->origin.y * 0.5, 1.0);
        }
        
        // Handle negative coordinates
        if (u < 0) u += 1.0;
        if (v < 0) v += 1.0;
    }
    else if (obj->type == CYLINDER)
	{
		t_vec3 local_hit = sub_vec(point->origin, obj->origin);
		float theta = atan2(local_hit.z, local_hit.x);
		u = (theta + M_PI) / (2 * M_PI);
		v = local_hit.y * 0.1;
		v = fmod(v, 1.0);
		if (v < 0) v += 1.0;
	}
	else if (obj->type == CONE)
	{
		// Use SAME logic as handle_bump
		t_vec3 axis = normalize(obj->normal);
		t_vec3 apex = add_vec(obj->origin, mul_vec(axis, obj->height));
		t_vec3 local_hit = sub_vec(point->origin, apex);
		
		float theta = atan2(local_hit.z, local_hit.x);
		u = (theta + M_PI) / (2 * M_PI);
		
		float dist_from_apex = vec_length(local_hit);
		v = dist_from_apex / (obj->height * 1.41421f);
		
		u = fmax(0.0f, fmin(1.0f, u));
		v = fmax(0.0f, fmin(1.0f, v));
	}
    else
    {
        return obj->color;
    }
    
    // Sample texture for COLOR (not just bump)
    int tex_x = (int)(u * obj->bump_texture->width);
    int tex_y = (int)(v * obj->bump_texture->height);
    
    tex_x = fmax(0, fmin(tex_x, obj->bump_texture->width - 1));
    tex_y = fmax(0, fmin(tex_y, obj->bump_texture->height - 1));
    
    uint8_t *pixels = obj->bump_texture->pixels;
    int index = (tex_y * obj->bump_texture->width + tex_x) * 4;
    
    // Return the actual texture colors
    t_color texture_color;
    texture_color.r = pixels[index];         // Red channel
    texture_color.g = pixels[index + 1];     // Green channel  
    texture_color.b = pixels[index + 2];     // Blue channel
    
    return texture_color;
}

t_vec3 reflect(t_vec3 incident, t_vec3 normal)
{
	return sub_vec(incident, mul_vec(normal, 2 * dot(incident, normal)));
}

void init_l_s(t_l_s *data)
{
	data->shininess = 500;
	data->specular_strength = 0.5;
	data->color.r = 0;
	data->color.g = 0;
	data->color.b = 0;
	data->lfar9 = 1.0f;
}

void	color_handle_help(t_minirt *data, t_l_s *s_l_data, t_color *obj, t_color *c_light)
{
	s_l_data->color.r += obj->r * c_light->r / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
	s_l_data->color.g += obj->g * c_light->g / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
	s_l_data->color.b += obj->b * c_light->b / 255.0f * s_l_data->lfar9 * s_l_data->light->ratio;
}

void color_handle_ambient(t_minirt *data, t_l_s *s_l_data, t_color *obj, t_color *c_light)
{
    s_l_data->color.r += obj->r * c_light->r / 255.0f * data->ambient.ratio;
    s_l_data->color.g += obj->g * c_light->g / 255.0f * data->ambient.ratio;
    s_l_data->color.b += obj->b * c_light->b / 255.0f * data->ambient.ratio;
}

void	clr_reflection(t_l_s *s_l_data)
{
	s_l_data->color.r += s_l_data->light->color.r / s_l_data->specular_strength * s_l_data->spec * s_l_data->light->ratio;
	s_l_data->color.g += s_l_data->light->color.g / s_l_data->specular_strength * s_l_data->spec * s_l_data->light->ratio;
	s_l_data->color.b += s_l_data->light->color.b / s_l_data->specular_strength * s_l_data->spec * s_l_data->light->ratio;
}

int handle_light_shadow(t_minirt *data, t_point *point, t_vec3 normal)
{
    t_l_s s_l_data;

    init_l_s(&s_l_data);
    s_l_data.light = data->light;
    
    // Get the texture color for this point
    t_color surface_color;
    if (point->obj->texture == CHECKER)
        surface_color = point->color;  // Use checker color
    else if (point->obj->texture == BUMP)
        surface_color = get_texture_color(point, point->obj);  // Use texture color
    else
        surface_color = point->obj->color;  // Use base object color
    
    while (s_l_data.light)
    {
        s_l_data.light_dir_n = normalize(sub_vec(s_l_data.light->origin, point->origin));
        if (!is_shadow(data, *point, s_l_data.light_dir_n, s_l_data.light->origin))
        {
            s_l_data.lfar9 = fmax(0.0f, dot(normal, s_l_data.light_dir_n));
            s_l_data.reflect_dir = reflect(mul_vec(s_l_data.light_dir_n, -1), normal);
            s_l_data.view_dir = normalize(sub_vec(data->camera.origin, point->origin));
            s_l_data.spec = powf(fmax(dot(s_l_data.view_dir, s_l_data.reflect_dir), 0.0f), s_l_data.shininess);
            
            // Use the surface color we calculated above
            color_handle_help(data, &s_l_data, &surface_color, &s_l_data.light->color);
            clr_reflection(&s_l_data); 
        }
        s_l_data.light = s_l_data.light->next;
    }
    
    // Apply ambient lighting with surface color
    color_handle_ambient(data, &s_l_data, &surface_color, &data->ambient.color);
    
    s_l_data.color.r = fmin(s_l_data.color.r, 255.0f);
    s_l_data.color.g = fmin(s_l_data.color.g, 255.0f);
    s_l_data.color.b = fmin(s_l_data.color.b, 255.0f);
    uint32_t a = 255;
    return ((int)s_l_data.color.r << 24) | ((int)s_l_data.color.g << 16) | ((int)s_l_data.color.b << 8) | a;
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
		point->color.r = 255;
		point->color.g = 255;
		point->color.b = 255;
	}
	else
	{
		point->color.r = 0;
		point->color.g = 0;
		point->color.b = 0;
	}
}
t_vec3 get_cylinder_normal(t_vec3 intersection_point, t_object *cylinder)
{
    // Cylinder axis (normalized)
    t_vec3 axis = normalize(cylinder->normal);
    
    // Vector from cylinder base to intersection point
    t_vec3 base_to_point = sub_vec(intersection_point, cylinder->origin);
    
    // Project intersection point onto the axis
    float projection_length = dot(base_to_point, axis);
    
    // Find the closest point on the axis
    t_vec3 axis_point = add_vec(cylinder->origin, mul_vec(axis, projection_length));
    
    // Normal is from axis point to intersection point
    t_vec3 normal = normalize(sub_vec(intersection_point, axis_point));
    
    return normal;
}
t_vec3 handle_bump(t_point *point, t_object *obj, t_vec3 original_normal)
{
    if (!obj->bump_texture)
        return original_normal;
    
    // Calculate texture coordinates based on object type
    float u, v;
    
    if (obj->type == SPHERE) 
    {
        t_vec3 hit = normalize(sub_vec(point->origin, obj->origin));
        u = 0.5 + atan2(hit.z, hit.x) / (2 * M_PI);
		v = 0.5 - asin(hit.y) / M_PI;
		// Clamp to [0,1] instead of fmod
		u = fmax(0.0f, fmin(1.0f, u));
		v = fmax(0.0f, fmin(1.0f, v));
    }
    else if (obj->type == PLANE) 
    {
		// Determine plane orientation based on normal
		t_vec3 plane_normal = normalize(obj->normal);
		
		if (fabs(plane_normal.y) > 0.9f) 
		{
			// Horizontal plane (floor/ceiling) - use X and Z
			u = fmod(point->origin.x * 0.5, 1.0);
			v = fmod(point->origin.z * 0.5, 1.0);
		}
		else if (fabs(plane_normal.x) > 0.9f) 
		{
			// Vertical plane facing X - use Y and Z  
			u = fmod(point->origin.z * 0.5, 1.0);
			v = fmod(point->origin.y * 0.5, 1.0);
		}
		else 
		{
			// Vertical plane facing Z - use X and Y
			u = fmod(point->origin.x * 0.5, 1.0);
			v = fmod(point->origin.y * 0.5, 1.0);
		}
		
		// Handle negative coordinates
		if (u < 0) u += 1.0;
		if (v < 0) v += 1.0;
    }
    else if (obj->type == CYLINDER)
    {
        t_vec3 local_hit = sub_vec(point->origin, obj->origin);
        float theta = atan2(local_hit.z, local_hit.x);
        u = (theta + M_PI) / (2 * M_PI);
        v = local_hit.y * 0.1;  // Scale factor
        v = fmod(v, 1.0);
        if (v < 0) v += 1.0;
    }
    else if (obj->type == CONE)
	{
		t_vec3 axis = normalize(obj->normal);
		t_vec3 apex = add_vec(obj->origin, mul_vec(axis, obj->height));
		t_vec3 local_hit = sub_vec(point->origin, apex);
		
		// Cylindrical coordinates from apex
		float theta = atan2(local_hit.z, local_hit.x);
		u = (theta + M_PI) / (2 * M_PI);
		
		// V coordinate based on distance from apex
		float dist_from_apex = vec_length(local_hit);
		v = dist_from_apex / (obj->height * 1.41421f);  // Normalize by max distance
		
		// Clamp to [0,1]
		u = fmax(0.0f, fmin(1.0f, u));
		v = fmax(0.0f, fmin(1.0f, v));
	}
    else
    {
        return original_normal;
    }
    
    // Sample the bump texture
    int tex_x = (int)(u * obj->bump_texture->width);
    int tex_y = (int)(v * obj->bump_texture->height);
    
    // Now clamp to valid bounds
    tex_x = fmax(0, fmin(tex_x, obj->bump_texture->width - 1));
    tex_y = fmax(0, fmin(tex_y, obj->bump_texture->height - 1));
    
    uint8_t *pixels = obj->bump_texture->pixels;
    int index = (tex_y * obj->bump_texture->width + tex_x) * 4;
    
    // Get RGB values and convert to normal perturbations
    float r = (pixels[index] / 255.0f - 0.5f) * 2.0f;     // -1 to 1
    float g = (pixels[index + 1] / 255.0f - 0.5f) * 2.0f; // -1 to 1  
    float b = (pixels[index + 2] / 255.0f - 0.5f) * 2.0f; // -1 to 1
    
    // Calculate tangent space vectors
    t_vec3 tangent, bitangent;
    
    // Choose tangent based on surface normal
    if (fabs(original_normal.y) < 0.9f)
        tangent = normalize(cross(original_normal, (t_vec3){0, 1, 0}));
    else
        tangent = normalize(cross(original_normal, (t_vec3){1, 0, 0}));
    
    bitangent = normalize(cross(original_normal, tangent));
    
    // Apply color-based bump perturbations
    float bump_strength = 0.03f;  // Adjust strength (0.05-0.3)
    
    t_vec3 bump_normal = add_vec(
        add_vec(
            mul_vec(tangent, r * bump_strength),
            mul_vec(bitangent, g * bump_strength)
        ),
        mul_vec(original_normal, 1.0f + b * bump_strength * 0.5f)
    );
    
    return normalize(bump_normal);
}
uint32_t get_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}
void rays_setup(t_minirt *data)
{
    int i, j;
    t_point point;
    t_vec3 normal;
    float intensity;
    float   max;
    int color;

    i = 0;
    max = 0;
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
                	normal = get_cylinder_normal(point.origin, point.obj);
				else if (point.obj->type == CONE)
				{
					// Simple radial normal calculation (like cylinder, but with slight tilt)
					t_vec3 axis = normalize(point.obj->normal);
					t_vec3 base_to_point = sub_vec(point.origin, point.obj->origin);
					
					// Project to find point on axis
					float height_proj = dot(base_to_point, axis);
					t_vec3 axis_point = add_vec(point.obj->origin, mul_vec(axis, height_proj));
					
					// Simple outward normal
					normal = normalize(sub_vec(point.origin, axis_point));
				}
            	if (point.obj->texture == CHECKER)
                	handle_checker(&point, point.obj);
				else if (point.obj->texture == BUMP)
					normal = handle_bump(&point, point.obj, normal);
                color = handle_light_shadow(data, &point, normal);
                mlx_put_pixel(data->img, i, j, color);
            }
			else
			{
				color = get_rgba(0, 0, 0, 255);
				mlx_put_pixel(data->img, i, j, color);
			}
            j++;
        }
        i++;
    }
}
