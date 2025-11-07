#include "Wall.h"
#include <math.h>
#include "../Imageio/Imageio.h"

Intersect_Cond Wall::intersection_check(const M3DVector3f start, const M3DVector3f dir, float& distance, M3DVector3f intersection_p)
{
	if (_tr1.intersection_check(start, dir, distance, intersection_p) == _k_hit)
	{
		return _k_hit;
	}
	else
	{
		return _tr2.intersection_check(start, dir, distance, intersection_p);
	}
}


void	Wall::shade(M3DVector3f view, M3DVector3f intersect_p, const Light& sp_light, M3DVector3f am_light, M3DVector3f color, bool shadow)
{
	/*for (int i = 0; i < 3; i++)
	{
		color[i] = _color[i];
	}*/

    // n: surface normal at the hit point (using second _tr1 triangle's normal)
    M3DVector3f n;
    _tr2.normal(n);                 // normal of the wall plane
    m3dNormalizeVector(n);

    // Optional: flipping normal so it faces the viewer
    if (m3dDotProduct(n, view) < 0.0f)
    {
        m3dScaleVector3(n, -1.0f);
    }

    // l: vector from light to point (matching the convention of Sphere::shade())
    M3DVector3f l, r, tmp_n;
    M3DVector3f I;      // light intensity (RGB)
    M3DVector3f l_pos;  // light position

    // Get light position and color/intensity
    sp_light.get_light(l_pos, I);

    // Build vectors
    m3dSubtractVectors3(l, intersect_p, l_pos);  // light -> point
    m3dNormalizeVector(l);

    // Calculating Reflection r = 2(l·n)n - l
    m3dCopyVector3(tmp_n, n);
    m3dScaleVector3(tmp_n, 2.0f * m3dDotProduct(l, n));
    m3dSubtractVectors3(r, tmp_n, l);
    m3dNormalizeVector(r);

    // If we want to respect hard shadows, we early-out to ambient only:
     /*if (shadow) {
         for (int i = 0; i < 3; ++i)
             color[i] = _color[i] * (_ka * am_light[i]);
         return;
     }*/

    // Phong components (matching the sphere’s style)
    for (int i = 0; i < 3; ++i)
    {
        float ambient = _ka * am_light[i];
        float diffuse = _kd * I[i] * m3dDotProduct(l, n);
        float specular = _ks * I[i] * m3dDotProduct(view, r);

        // Prevent negative contributions
        if (diffuse < 0.0f) diffuse = 0.0f;
        if (specular < 0.0f) specular = 0.0f;

        // Base wall color contributes to ambient + diffuse; specular is “white-ish”
        color[i] = _color[i] + diffuse + specular + ambient;

        // Optional softening when shadowed (similar to the sphere approach)
        if (shadow) color[i] *= 0.3f;

        // Clamp
        /*if (color[i] > 1.0f) color[i] = 1.0f;
        if (color[i] < 0.0f) color[i] = 0.0f;*/
    }
}


void	Wall::get_reflect_direct(const M3DVector3f direct0, const M3DVector3f intersect_p, M3DVector3f reflect_direct)
{

}


void Wall::load_texture(std::string file_name)
{

}

void Wall::texture_color(M3DVector3f pos, M3DVector3f color)
{
}

void	Wall::get_texel(float x, float y, M3DVector3f color)
{
	return;
}