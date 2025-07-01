

#include "../minirt.h"
#include <math.h>

float vec_length(t_vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
t_vec3 normalize(t_vec3 v) {
    float len = vec_length(v);
    if (len == 0.0f)
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
    return ((float){a.x * b.x + a.y * b.y +a.z * b.z});
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
    aspect_ratio = 800.0 / 800.0;
    fov_rad = tan(data->camera.fov / 2 * M_PI / 180);

    screen_x = (2 * (x + 0.5) / 800 - 1) * aspect_ratio * fov_rad;
    screen_y = (1 - 2 * (y + 0.5) / 800) * fov_rad;

    t_vec3 forward = normalize(data->camera.normal);
    t_vec3 world_up = {0, 1, 0};
    t_vec3 right = normalize(cross(world_up, forward));
    t_vec3 up = cross(forward, right);
    ray_direction = normalize(
        add_vec(forward, 
        add_vec(mul_vec(right, screen_x),
        mul_vec(up, screen_y))));
    return ray_direction;
}

float intersect_sphere(t_minirt *data, t_vec3 ray_direction, int i , int j)
{

    t_vec3 L = sub_vec(data->camera.origin, data->objects->origin);
    float radius = data->objects->diameter/2.0f;
    float a = 1.0f;
    float b = 2.0f * dot(L, ray_direction);
    float c = dot(L, L) - radius * radius;

    float discriminant = b*b - 4*a*c;

    if (discriminant < 0)
        return -1.0f;

    float sqrt_discriminant = sqrtf(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2*a);
    float t2 = (-b + sqrt_discriminant) / (2*a);

    if (t1 > 0.001f)
    {
        my_mlx_p_pix(234723402, j, i, data);
        return t1;
    }
    if (t2 > 0.001f)
        return t2;

    return -1.0f;

}

float intersect_plane(t_minirt *data, t_vec3 ray_direction)
{
    float denom = dot(data->objects->normal, ray_direction);

    if (fabs(denom) < 1e-6)
        return -1;

    t_vec3 L = sub_vec(data->objects->origin, data->camera.origin);
    float t = dot(data->objects->origin, L) / denom;
    if (t < 0)
        return -1;
    return t;
}
t_vec3 find_closest_inter(t_minirt *data, t_vec3 ray_direction, int x, int y)
{
    t_object *current = data->objects;
    t_vec3 closest_point = {0, 0, 0};
    t_vec3 new_point;
    float distance = -1.0f;
    float closest_distance = MAX_F;

    while (current != NULL)
    {
        if (current->type == SPHERE) {
            distance = intersect_sphere(data, ray_direction, x, y);
        }
        else if (current->type == PLANE) {
            distance = intersect_plane(data, ray_direction);
        }
        else if (current->type == CYLINDER) {
            // new_point = intersect_cylinder();
        }
        if (distance > 0.001f && distance < closest_distance) {
            closest_point = new_point;
            closest_distance = distance;
        }
        current = current->next;
    }
    return closest_point;
}
void rays_setup(t_minirt *data)
{
    int i;
    int j;

    i = 0;
    while (i < 800)
    {
        j = 0;
        while (j < 800)
        {
            t_vec3 dir_ray = generate_rays(data, i, j);
            t_vec3 closest_point = find_closest_inter(data, dir_ray, i, j);
            // if (has_intersec()) {
                // handl_light();
            // }
            // else {
            //     get_background_col();
            // }
            j++;
        }
        i++;
    }
}
