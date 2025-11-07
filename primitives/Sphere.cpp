#include "sphere.h"
#include "../Imageio/Imageio.h"
#include <math.h>

Intersect_Cond	Sphere::intersection_check(const M3DVector3f start, const M3DVector3f dir, float& distance, M3DVector3f intersection_p)
{
	M3DVector3f ray_to_center;
	m3dSubtractVectors3(ray_to_center, _pos, start);
	float ray_center_length = m3dDotProduct(ray_to_center, ray_to_center);

	float closest_point = m3dDotProduct(ray_to_center, dir);
	if (closest_point < 0)
	{
		return _k_miss;
	}

	float halfCord2 = (_rad * _rad) - (ray_center_length - (closest_point * closest_point));
	if (halfCord2 < 0)
	{
		return _k_miss;
	}

	Intersect_Cond type;
	M3DVector3f tmp;
	m3dSubtractVectors3(tmp, start, _pos);
	float length = m3dDotProduct(tmp, tmp);
	if (length < _rad2)
	{
		type = _k_inside;
		distance = closest_point + sqrt(halfCord2);
	}
	else
	{
		type = _k_hit;
		distance = closest_point - sqrt(halfCord2);
	}

	M3DVector3f tmp_v;
	m3dCopyVector3(tmp_v, dir);
	m3dScaleVector3(tmp_v, distance);
	m3dAddVectors3(intersection_p, start, tmp_v);

	return type;
}

void Sphere::shade(M3DVector3f view,
    M3DVector3f intersect_p,
    const Light& sp_light,
    M3DVector3f am_light,
    M3DVector3f color,
    bool shadow)
{
    // --- Surface normal at hit point (pointing outward) ---
    M3DVector3f n;
    m3dSubtractVectors3(n, intersect_p, _pos);    // n = P - center
    m3dNormalizeVector(n);

    // Optional: making sure the normal faces the viewer
    /*if (m3dDotProduct(n, view) < 0.0f)
        m3dScaleVector3(n, -1.0f);*/

    // --- Light position & intensity ---
    M3DVector3f l_pos, I;
    sp_light.get_light(l_pos, I);

    // --- Vector from light to point (match the convention used for Wall) ---
    M3DVector3f l;
    m3dSubtractVectors3(l, intersect_p, l_pos);   // l = P - Lpos (light -> point)
    m3dNormalizeVector(l);

    // --- Reflection vector r = 2(l·n)n - l ---
    M3DVector3f r, tmp_n;
    m3dCopyVector3(tmp_n, n);
    m3dScaleVector3(tmp_n, 2.0f * m3dDotProduct(l, n));
    m3dSubtractVectors3(r, tmp_n, l);
    m3dNormalizeVector(r);

    // --- Phong components (ambient + diffuse + specular) ---
    for (int i = 0; i < 3; ++i)
    {
        float ambient = _ka * am_light[i];
        float diffuse = _kd * I[i] * m3dDotProduct(l, n);
        float specular = _ks * I[i] * m3dDotProduct(view, r);

        if (diffuse < 0.0f) diffuse = 0.0f;
        if (specular < 0.0f) specular = 0.0f;

        // Base color scales ambient+diffuse; specular adds highlight
        color[i] = _color[i] + diffuse + specular + ambient;

        // Optional: soften when in shadow (keeps some ambient)
        //if (shadow) color[i] *= 0.3f;

        // Clamp to [0, 1]
        /*if (color[i] > 1.0f) color[i] = 1.0f;
        if (color[i] < 0.0f) color[i] = 0.0f;*/
    }
}
