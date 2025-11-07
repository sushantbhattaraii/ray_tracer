#include "Scene.h"


Scene::Scene()
{
}

Scene::~Scene(void)
{
	Basic_Primitive* prim = NULL;
	for (Prim_List::iterator it = _prim_list.begin(); it != _prim_list.end();)
	{
		prim = *it;
		_prim_list.erase(it++);
		delete prim;
	}
}

void	Scene::assemble()
{
	M3DVector3f	x0y0z0 = { 0, 0, 0 };
	M3DVector3f	x1y0z0 = { _dim[0], 0, 0 };
	M3DVector3f	x0y1z0 = { 0,_dim[1], 0 };
	M3DVector3f	x1y1z0 = { _dim[0], _dim[1], 0 };
	M3DVector3f	x0y0z1 = { 0,0,_dim[2] };
	M3DVector3f	x1y0z1 = { _dim[0],0,_dim[2] };
	M3DVector3f	x0y1z1 = { 0,_dim[1],_dim[2] };
	M3DVector3f	x1y1z1 = { _dim[0],_dim[1],_dim[2] };



	//lighting setup.
	M3DVector3f pos;
	M3DVector3f color;

	for (int i = 0; i < 3; i++) {
		pos[i] = (float)(rand()) / (float)(RAND_MAX);
		color[i] = (float)(rand()) / (float)(RAND_MAX);
		_amb_light[i] = (float)(rand()) / (float)(RAND_MAX);
	}

	/*pos[0] = 0.75;
	pos[1] = 1.0;
	pos[2] = 0;

	color[0] = 0.75;
	color[1] = 1.0;
	color[2] = 0;*/

	_sp_light.set_light(pos, color);


	M3DVector3f wall_color_left = { 1.0,1.0,0.0 };
	//Walls
	Wall* left_wall = new Wall(x0y1z0, x0y1z1, x0y0z1, x0y0z0, wall_color_left);
	_prim_list.push_back(left_wall); // Left wall

	M3DVector3f wall_color_right = { 2.0 / 255, 112.0 / 255, 28.0 / 255 };
	Wall* right_wall = new Wall(x1y1z1, x1y1z0, x1y0z0, x1y0z1, wall_color_right);
	_prim_list.push_back(right_wall);	// Right wall

	M3DVector3f wall_color = { 1.0, 0.4, 0.9 };
	Wall* top_wall = new Wall(x1y1z1, x0y1z1, x0y1z0, x1y1z0, wall_color);
	_prim_list.push_back(top_wall);	// Top wall

	M3DVector3f wall_color_bottom = { 0, 0, 1.0 };
	Wall* bottom_wall = new Wall(x1y0z1, x1y0z0, x0y0z0, x0y0z1, wall_color_bottom);
	bottom_wall->set_properties(0.9, 0, 0.9, 0);
	_prim_list.push_back(bottom_wall);	// Bottom wall

	M3DVector3f back_wall_color = { 243. / 255, 173. / 255, 32. / 255 };
	Wall* back_wall = new Wall(x1y1z0, x0y1z0, x0y0z0, x1y0z0, back_wall_color);
	_prim_list.push_back(back_wall);	// Back wall
	//End of walls

	//Spheres
	float	rad = _dim[2] / 4.0;
	M3DVector3f sphere_color1 = { 36.0 / 255, 224.0 / 255, 238.0 / 255 };
	M3DVector3f sphere_center1 = { _dim[0] - rad - 20, rad, _dim[2] * 2.0 / 3.0 - rad };
	Sphere* sp1 = new Sphere(sphere_center1, rad, sphere_color1);
	sp1->set_properties(0.0, 0.9, 0.0, 0.9);
	_prim_list.push_back(sp1);

	rad /= 1.5;
	M3DVector3f sphere_center2 = { rad + 20, rad, rad + 20 };
	M3DVector3f sphere_color2 = { 0.5, 0.5, 0.5 };
	Sphere* sp2 = new Sphere(sphere_center2, rad, sphere_color2);
	sp2->set_properties(0.0, 0, 0.0, 0);
	_prim_list.push_back(sp2);
	//End of Spheres

}

Intersect_Cond	Scene::intersection_check(const M3DVector3f start, const M3DVector3f dir, Basic_Primitive** prim_intersect, M3DVector3f closest_point)
{
	Basic_Primitive* prim = NULL;
	float		distance;
	M3DVector3f point;

	float	min_distance = 0x0ffffff;
	*prim_intersect = NULL;

	Intersect_Cond ret = _k_miss;
	Intersect_Cond tmp;
	for (Prim_List::iterator it = _prim_list.begin(); it != _prim_list.end(); it++)
	{
		prim = *it;
		tmp = prim->intersection_check(start, dir, distance, point);
		if (tmp != _k_miss)
		{
			if (distance < min_distance)
			{
				min_distance = distance;
				*prim_intersect = prim;
				m3dCopyVector3(closest_point, point);
				ret = tmp;
			}
		}
	}

	return ret;
}