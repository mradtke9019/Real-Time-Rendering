#pragma once
#include "Shader.h"
#include "Texture.h"
class KnotShader : public Shader
{
private:
	float rmin = 104.06;
	float rmax = 143.375;
	float hmax = 4000;
	float knum = 108;

	// Tree geo maps
	Texture knot_height_map;
	Texture knot_orientation_map;
	Texture knot_state_map;
	Texture pith_and_radius_map;

	// Wood color maps
	Texture wood_bar_color;
	Texture wood_bar_normal;
	Texture wood_bar_specular;

	std::vector<Texture> textures;

	void LoadDefaultWoodTextures()
	{
		textures = std::vector<Texture>();
		// Tree geo maps
		knot_height_map = Texture::TextureFromFile("knot_height_map.bmp", "./Images/tree_geo_maps", "KnotHeightMap", "KnotHeightMap");
		knot_orientation_map = Texture::TextureFromFile("knot_orientation_map.bmp", "./Images/tree_geo_maps", "KnotOrientMap", "KnotOrientMap");
		knot_state_map = Texture::TextureFromFile("knot_state_map.bmp", "./Images/tree_geo_maps", "KnotStateMap", "KnotStateMap");
		pith_and_radius_map = Texture::TextureFromFile("pith_and_radius_map.bmp", "./Images/tree_geo_maps", "PithRadiusMap", "PithRadiusMap");

		// Wood color maps
		wood_bar_color = Texture::TextureFromFile("wood_bar_color.bmp", "./Images/wood_color_maps", "ColorMap", "ColorMap");
		wood_bar_normal = Texture::TextureFromFile("wood_bar_normal.bmp", "./Images/wood_color_maps", "NormalMap", "NormalMap");
		wood_bar_specular = Texture::TextureFromFile("wood_bar_specular.bmp", "./Images/wood_color_maps", "SpecularMap", "SpecularMap");

		textures.push_back(knot_height_map);
		textures.push_back(knot_orientation_map);
		textures.push_back(knot_state_map);
		textures.push_back(pith_and_radius_map);
		textures.push_back(wood_bar_color);
		textures.push_back(wood_bar_normal);
		textures.push_back(wood_bar_specular);
	}

public:

	KnotShader(const char* vertexShaderPath, const char* fragmentShaderPath);


	void SetRMin(float rmin);

	void SetRMax(float rmax);

	void SetHMax(float hmax);

	void SetKNum(float knum);

	void Use();
};

