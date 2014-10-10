#include <memory>
#include <string>
#include <regex>
#include <tinyxml2.h>
#include "textures/constant_texture.h"
#include "textures/image_texture.h"
#include "textures/uv_texture.h"
#include "textures/checkerboard_texture.h"
#include "textures/uv_mapping.h"
#include "textures/scale_texture.h"
#include "loaders/load_scene.h"
#include "loaders/load_texture.h"

Texture* load_texture(tinyxml2::XMLElement *elem, const std::string &mat_name,
	TextureCache &cache, const std::string &file)
{
	using namespace tinyxml2;
	//Read the color as well to check if it's a scale texture we want
	Colorf color{1, 1, 1};
	read_color(elem, color);
	std::string name, gen_name;
	if (elem->Attribute("texture") && color == Colorf{1, 1, 1}){
		name = elem->Attribute("texture");
	}
	else if (elem->Attribute("texture")){
		name = elem->Attribute("texture");
		gen_name = "__" + mat_name + "_" + elem->Value() + "_tex";
	}
	else {
		gen_name = "__" + mat_name + "_" + elem->Value() + "_tex";
	}
	Texture *tex = cache.get(name);
	//If we've already loaded the texture and we're not scaling we can just return it
	if (tex && gen_name.empty()){
		std::cout << "Using cached texture: " << name << std::endl;
		return tex;
	}
	//If it's just a generated texture we're just setting a constant color for a material
	if (name.empty()){
		cache.add(gen_name, std::make_unique<ConstantTexture>(color));
		tex = cache.get(gen_name);
		name = gen_name;
	}
	//We need to load a texture file or procedural texture
	else {
		Transform transform;
		read_transform(elem, transform);
		transform = transform.inverse();

		std::regex match_file{".*\\.[a-zA-Z]{3}$"};
		std::smatch match;
		//If we're applying a scaling to an existing texture
		if (tex != nullptr && !gen_name.empty()){
			std::cout << "Applying scaling to cached texture: " << name << std::endl;
			cache.add(gen_name, std::make_unique<ConstantTexture>(color));
			Texture &b = *cache.get(gen_name);
			name = gen_name + name + "_scaled";
			cache.add(name, std::make_unique<ScaleTexture>(*tex, b));
			tex = cache.get(name);
		}
		else if (std::regex_match(name, match, match_file)){
			std::string tex_file = file.substr(0, file.rfind(PATH_SEP) + 1) + name;
			cache.add(name, std::make_unique<ImageTexture>(tex_file,
				std::make_unique<UVMapping>(transform)));
			//If we're also applying some scaling via a color create the constant texture and return the scale texture
			if (!gen_name.empty()){
				cache.add(gen_name, std::make_unique<ConstantTexture>(color));
				Texture &a = *cache.get(name);
				Texture &b = *cache.get(gen_name);
				name = gen_name + name + "_scaled";
				cache.add(name, std::make_unique<ScaleTexture>(a, b));
			}
			tex = cache.get(name);
		}
		else if (name == "uv"){
			name = "__" + mat_name + "_" + name + "_tex";
			cache.add(name, std::make_unique<UVTexture>(std::make_unique<UVMapping>(transform)));
			tex = cache.get(name);
			//If we're also applying some scaling via a color create the constant texture and return the scale texture
			if (!gen_name.empty()){
				cache.add(gen_name, std::make_unique<ConstantTexture>(color));
				Texture &a = *cache.get(name);
				Texture &b = *cache.get(gen_name);
				name = name + "_scaled";
				cache.add(name, std::make_unique<ScaleTexture>(a, b));
			}
			tex = cache.get(name);
		}
		else if (name == "checkerboard"){
			Colorf a, b{1};
			//My checkerboard colors are flipped compared to what Cem's expect
			XMLElement *c = elem->FirstChildElement("color1");
			if (c){
				read_color(c, b);
			}
			c = elem->FirstChildElement("color2");
			if (c){
				read_color(c, a);
			}
			name = "__" + mat_name + "_" + name + "_tex";
			cache.add(name, std::make_unique<CheckerboardTexture>(a, b, std::make_unique<UVMapping>(transform)));
			//If we're also applying some scaling via a color create the constant texture and return the scale texture
			if (!gen_name.empty()){
				cache.add(gen_name, std::make_unique<ConstantTexture>(color));
				Texture &a = *cache.get(name);
				Texture &b = *cache.get(gen_name);
				name = name + "_scaled";
				cache.add(name, std::make_unique<ScaleTexture>(a, b));
			}
			tex = cache.get(name);
		}
		else {
			std::cout << "Warning: procedural texture " << name << " not implemented\n";
			cache.add(name, std::make_unique<ConstantTexture>(Colorf{0, 0, 0}));
			tex = cache.get(name);
		}
	}
	std::cout << "Created texture name: " << name << std::endl;
	return tex;
}

