

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

float intersect_sphere(t_minirt *data, t_vec3 ray_direction, t_object *current)
{

    t_vec3 L = sub_vec(data->camera.origin, current->origin);
    float radius = current->diameter/2.0f;
    float a = 1.0f;
    float b = 2.0f * dot(ray_direction,L);
    float c = dot(L, L) - radius * radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0)
        return -1.0f;

    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2*a);
    float t2 = (-b + sqrt_discriminant) / (2*a);
    if (t1 > 0.001f)
        return t1;
    if (t2 > 0.001f)
        return t2;
    return -1.0f;

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
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;
    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2 * a);
    float t2 = (-b + sqrt_discriminant) / (2 * a);

    if (t1 > 0.001f)
    {
        t_vec3 intersection_point = add_vec(data->camera.origin, mul_vec(ray_direction, t1));
        if (intersection_point.y >= current->origin.y && intersection_point.y <= current->origin.y + current->height)
            return t1;
    }
    if (t2 > 0.001f)
    {
        t_vec3 intersection_point = add_vec(data->camera.origin, mul_vec(ray_direction, t2));
        if (intersection_point.y >= current->origin.y && intersection_point.y <= current->origin.y + current->height)
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
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;
    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2 * a);
    float t2 = (-b + sqrt_discriminant) / (2 * a);
    if (t1 > 0.001f)
    {
        t_vec3 intersection_point = add_vec(ray_origin, mul_vec(ray_direction, t1));
        if (intersection_point.y >= current->origin.y && intersection_point.y <= current->origin.y + current->height)
            return t1;
    }
    if (t2 > 0.001f)
    {
        t_vec3 intersection_point = add_vec(ray_origin, mul_vec(ray_direction, t2));
        if (intersection_point.y >= current->origin.y && intersection_point.y <= current->origin.y + current->height)
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

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;

    float sqrt_d = sqrtf(discriminant);
    float t1 = (-b - sqrt_d) / (2.0f * a);
    float t2 = (-b + sqrt_d) / (2.0f * a);

    if (t1 > 0.001f)
        return t1;
    if (t2 > 0.001f)
        return t2;
    return -1.0f;
}

int is_shadow(t_minirt *data, t_point point, t_vec3 light_dir_n)
{
    float   max_dstance;
    float   dstance;
    t_object    *obj;

    max_dstance = vec_length(sub_vec(data->light->origin, point.origin));
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
                // dstance = intersect_plane_shadow(data, light_dir_n, obj);
            if (dstance > 0.0f && dstance < max_dstance)
                return (1);
        }
        obj = obj->next;
    }
    return (0);
}

float    handle_light_shadow(t_minirt *data, t_point *point, t_vec3 normal)
{
    t_light *light;
    float   intensity;
    t_vec3  light_dir_n;

    light = data->light;
    intensity = 0;
    while (light)
    {
        light_dir_n = normalize(sub_vec(light->origin, point->origin));
        if (!is_shadow(data, *point, light_dir_n))
            intensity += fmax(0.0f, dot(normal, light_dir_n));
        light = light->next;
    }
    intensity += data->ambient.ratio;
    intensity = fmin(1.0f, intensity);
    return (intensity);
}

void rays_setup(t_minirt *data)
{
    int i, j;
    t_point point;
    t_vec3 normal;
    t_vec3 light_dir_n;
    t_vec3  light;
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
                // light_dir_n = normalize(sub_vec(data->light->origin, point.origin));
                // if (!is_shadow(data, point, light_dir_n))
                //     intensity = fmax(0.0f, dot(normal, light_dir_n));
                // else
                // {
                //     intensity = fmax(0.0f, dot(normal, light_dir_n)) * 0.75;
                //     // intensity = 0;
                // }
                // intensity += data->ambient.ratio;
                // intensity = fmin(1.0f, intensity);
                intensity = handle_light_shadow(data, &point, normal);
                int r = (int)(point.obj->R * intensity);
                int g = (int)(point.obj->G * intensity);
                int b = (int)(point.obj->B * intensity);
                color = (r << 16) | (g << 8) | b;
                my_mlx_p_pix(color, i, j, data);
            }
            else
                my_mlx_p_pix(0x000000, i, j, data);
            j++;
        }
        i++;
    }
}
