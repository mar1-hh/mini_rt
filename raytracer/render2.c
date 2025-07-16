#include "../minirt.h"
t_vec3 get_cone_normal(t_vec3 intersection_point, t_object *cone)
{
    t_vec3 axis;
    t_vec3 apex;
    t_vec3 base_to_point;
    float height_proj;
    t_vec3 axis_point;
    t_vec3 radial;
    float radius_at_height;
    t_vec3 normal;
    
    axis = normalize(cone->normal);
    apex = add_vec(cone->origin, mul_vec(axis, cone->height));
    base_to_point = sub_vec(intersection_point, cone->origin);
    height_proj = dot(base_to_point, axis);
    axis_point = add_vec(cone->origin, mul_vec(axis, height_proj));
    radial = normalize(sub_vec(intersection_point, axis_point));
    radius_at_height = cone->diameter / 2.0f * (cone->height - height_proj) / cone->height;
    normal = normalize(add_vec(radial, mul_vec(axis, radius_at_height / cone->height)));
    
    return normal;
}