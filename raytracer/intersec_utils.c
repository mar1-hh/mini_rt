#include "../minirt.h"

float handle_cylinder_degenerate(float c, t_vec3 ray_origin, t_vec3 ray_direction,
    t_object *current, t_vec3 axis)
{
    if (c >= 0)
        return -1.0f;
    return -1.0f; 
}

float check_cylinder_heights(float t1, float t2, t_vec3 ray_origin, 
    t_vec3 ray_direction, t_object *current, t_vec3 axis)
{
    float height = current->height;
    if (t1 > 0.001f)
    {
        t_vec3 p1 = add_vec(ray_origin, mul_vec(ray_direction, t1));
        t_vec3 to_p1 = sub_vec(p1, current->origin);
        float h1 = dot(to_p1, axis);
        if (h1 >= 0 && h1 <= height)
            return t1;
    }
    if (t2 > 0.001f)
    {
        t_vec3 p2 = add_vec(ray_origin, mul_vec(ray_direction, t2));
        t_vec3 to_p2 = sub_vec(p2, current->origin);
        float h2 = dot(to_p2, axis);
        if (h2 >= 0 && h2 <= height)
            return t2;
    }

    return -1.0f;
}
float intersect_sphere_unified(t_vec3 ray_origin, t_vec3 ray_direction, 
                              t_object *sphere)
{
    t_vec3 oc = sub_vec(ray_origin, sphere->origin);
    float radius = sphere->diameter / 2.0f;
    float a = dot(ray_direction, ray_direction);
    float b = 2.0f * dot(oc, ray_direction);
    float c = dot(oc, oc) - radius * radius;
    return check_descriminant(a, b, c);
}

float intersect_cylinder_unified(t_vec3 ray_origin, t_vec3 ray_direction,
                                t_object *current)
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
    return calculate_cylinder_intersection(a, b, c, ray_origin, 
                                         ray_direction, current, axis);
}

float calculate_cylinder_intersection(float a, float b, float c,
                                    t_vec3 ray_origin, t_vec3 ray_direction,
                                    t_object *current, t_vec3 axis)
{
    if (a < 1e-6f)
        return handle_cylinder_degenerate(c, ray_origin, ray_direction, 
                                        current, axis);
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0f;
    float sqrt_discriminant = sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);
    return check_cylinder_heights(t1, t2, ray_origin, ray_direction, 
                                current, axis);
}
