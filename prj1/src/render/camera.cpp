#include <limits>
#include <array>
#include "linalg/vector.h"
#include "linalg/point.h"
#include "linalg/ray.h"
#include "linalg/transform.h"
#include "render/camera.h"

//TODO: Is FLT_MAX fine to use for the far plane location? or should we pick an arbitrary but large value?
Camera::Camera(const Transform &cam_world, std::array<float, 4> img_screen, float fov, int xres, int yres)
	: cam_world(cam_world), cam_screen(Transform::perspective(fov, 1, std::numeric_limits<float>::max()))
{
	screen_raster = Transform::scale(static_cast<float>(xres), static_cast<float>(yres), 1)
		* Transform::scale(1 / (img_screen[1] - img_screen[0]), 1 / (img_screen[3] - img_screen[2]), 1)
		* Transform::translate(Vector{-img_screen[0], -img_screen[3], 0});
	raster_screen = screen_raster.inverse();
	raster_cam = cam_screen.inverse() * raster_screen;
}
Ray Camera::generate_ray(float x, float y) const {
	//Take the raster space position -> camera space
	Vector px_pos{x, y, 0};
	raster_cam(px_pos, px_pos);
	//Shoot ray from origin (camera pos) through the point
	Ray ray{Point{0, 0, 0}, px_pos};
	//Transform the ray to world space so it can be cast through the scene
	cam_world(ray, ray);
	return ray;
}
