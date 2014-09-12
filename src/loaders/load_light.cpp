#include <memory>
#include <string>
#include <tinyxml2.h>
#include "lights/ambient_light.h"
#include "lights/direct_light.h"
#include "lights/point_light.h"
#include "loaders/load_scene.h"
#include "loaders/load_light.h"

/*
 * Load the AmbientLight properties and return the light
 * elem should be the root of the ambient light being loaded
 */
static std::unique_ptr<Light> load_ambientl(tinyxml2::XMLElement *elem);
/*
 * Load the DirectLight properties and return the light
 * elem should be the root of the direct light being loaded
 */
static std::unique_ptr<Light> load_directl(tinyxml2::XMLElement *elem);
/*
 * Load the PointLight properties and return the light
 * elem should be the root of the direct light being loaded
 */
static std::unique_ptr<Light> load_pointl(tinyxml2::XMLElement *elem);

void load_lights(tinyxml2::XMLElement *elem, LightCache &cache){
	using namespace tinyxml2;
	for (XMLNode *n = elem; n; n = n->NextSibling()){
		if (n->Value() == std::string{"light"}){
			XMLElement *l = n->ToElement();
			std::string name = l->Attribute("name");
			std::cout << "loading light: " << name << std::endl;
			std::unique_ptr<Light> light;
			std::string type = l->Attribute("type");
			if (type == "ambient"){
				light = load_ambientl(l);
			}
			else if (type == "direct"){
				light = load_directl(l);
			}
			else if (type == "point"){
				light = load_pointl(l);
			}
			cache.add(name, std::move(light));
		}
		else {
			//The lights are all passed in a block, so once
			//we hit something not a light we're done loading
			return;
		}
	}
}
std::unique_ptr<Light> load_ambientl(tinyxml2::XMLElement *elem){
	Colorf color{1, 1, 1};
	read_color(elem->FirstChildElement("intensity"), color);
	color.normalize();
	std::cout << "AmbientLight intensity: " << color << std::endl;
	return std::unique_ptr<Light>{new AmbientLight{color}};
}
std::unique_ptr<Light> load_directl(tinyxml2::XMLElement *elem){
	Colorf color{1, 1, 1};
	Vector dir{0, 0, 0};
	read_color(elem->FirstChildElement("intensity"), color);
	read_vector(elem->FirstChildElement("direction"), dir);
	color.normalize();
	std::cout << "DirectLight intensity: " << color
		<< ", direction: " << dir << std::endl;
	return std::unique_ptr<Light>{new DirectLight{color, dir}};
}
std::unique_ptr<Light> load_pointl(tinyxml2::XMLElement *elem){
	Colorf color{1, 1, 1};
	Point pos{0, 0, 0};
	read_color(elem->FirstChildElement("intensity"), color);
	read_point(elem->FirstChildElement("position"), pos);
	color.normalize();
	std::cout << "PointLight intensity: " << color
		<< ", position: " << pos << std::endl;
	return std::unique_ptr<Light>{new PointLight{color, pos}};
}
