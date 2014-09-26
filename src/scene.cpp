#include <string>
#include "geometry/geometry.h"
#include "material/material.h"
#include "lights/light.h"
#include "render/render_target.h"
#include "render/camera.h"
#include "samplers/sampler.h"
#include "scene.h"

Scene::Scene(Camera camera, RenderTarget target, std::unique_ptr<Sampler> sampler, int depth)
	: camera(std::move(camera)), render_target(std::move(target)), sampler(std::move(sampler)),
	root(nullptr, nullptr, Transform{}, "root"), max_depth(depth)
{}
GeometryCache& Scene::get_geom_cache(){
	return geom_cache;
}
MaterialCache& Scene::get_mat_cache(){
	return mat_cache;
}
TextureCache& Scene::get_tex_cache(){
	return tex_cache;
}
LightCache& Scene::get_light_cache(){
	return light_cache;
}
Camera& Scene::get_camera(){
	return camera;
}
RenderTarget& Scene::get_render_target(){
	return render_target;
}
const RenderTarget& Scene::get_render_target() const {
	return render_target;
}
const Sampler& Scene::get_sampler() const {
	return *sampler;
}
Node& Scene::get_root(){
	return root;
}
int Scene::get_max_depth() const {
	return max_depth;
}

