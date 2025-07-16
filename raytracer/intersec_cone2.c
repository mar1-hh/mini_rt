#include "../minirt.h"

float calculate_cone_coefficients(t_cone_vars *vars, t_vec3 ray_origin,
                                t_vec3 ray_direction)
{
    vars->oa = sub_vec(ray_origin, vars->apex);
    vars->cos_squared = (vars->height * vars->height) / 
                       (vars->radius * vars->radius + vars->height * vars->height);
    vars->vdot_axis = dot(ray_direction, vars->axis);
    vars->oa_dot_axis = dot(vars->oa, vars->axis);
    return calculate_discriminant(vars, ray_direction);
}

float calculate_discriminant(t_cone_vars *vars, t_vec3 ray_direction)
{
    vars->a = dot(ray_direction, ray_direction) - vars->cos_squared
            * vars->vdot_axis * vars->vdot_axis;
    vars->b = 2.0f * (dot(ray_direction, vars->oa) - vars->cos_squared
            * vars->vdot_axis * vars->oa_dot_axis);
    vars->c = dot(vars->oa, vars->oa) - vars->cos_squared
            * vars->oa_dot_axis * vars->oa_dot_axis;
    return vars->b * vars->b - 4 * vars->a * vars->c;
}

int validate_cone_intersection(float t, t_vec3 ray_origin,
                             t_vec3 ray_direction, t_cone_vars *vars)
{
    if (t <= 0.001f)
        return 0;
    t_vec3 p = add_vec(ray_origin, mul_vec(ray_direction, t));
    t_vec3 ap = sub_vec(p, vars->apex);
    float proj = dot(ap, mul_vec(vars->axis, -1));
    return (proj >= 0 && proj <= vars->height);
}