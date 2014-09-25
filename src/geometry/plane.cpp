#include <cmath>
#include "linalg/vector.h"
#include "linalg/ray.h"
#include "geometry/plane.h"

bool Plane::intersect(Ray &ray, DifferentialGeometry &diff_geom){
	float n_dot_r = ray.d.dot(Vector{0, 0, 1});
	//If the ray is perpindicular to the normal there's no
	//way for it to hit the plane
	if (std::abs(n_dot_r) <= 0){
		return false;
	}
	//We're still treating the plane as infinite here so if it's not
	//perpindicular it definitely hits somewhere
	float t = -Vector{ray.o}.dot(Vector{0, 0, 1}) / n_dot_r;
	if (t <= ray.min_t || t >= ray.max_t){
		return false;
	}
	//It's in the range for the ray so now check if it's in range
	//for the finite plane
	Point hit = ray(t);
	if (hit.x >= -1 && hit.x <= 1 && hit.y >= -1 && hit.y <= 1){
		ray.max_t = t;
		diff_geom.point = hit;
		diff_geom.normal = Normal{0, 0, 1};
		if (ray.d.dot(diff_geom.normal) < 0){
			diff_geom.hit_side = HITSIDE::FRONT;
		}
		else {
			diff_geom.hit_side = HITSIDE::BACK;
		}
		return true;
	}
	return false;
}
BBox Plane::bound() const {
	return BBox{Point{-1, -1, 0}, Point{1, 1, 0}};
}
void Plane::refine(std::vector<Geometry*> &prims){
	prims.push_back(this);
}

