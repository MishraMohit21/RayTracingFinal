#pragma once

#include <glm/glm.hpp>

#include <vector>


struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;
	glm::vec3 EmissionColor{ 0.0f };
	float EmissionStrength = 0.0f;


	glm::vec3 GetEmission() const { return EmissionColor * EmissionStrength; }
};

struct Sphere
{
	glm::vec3 Position{ 0.0f };
	float Radius = 0.5f;
	int MaterialIndex = 0;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;

	Scene()
	{
		Material mat;
		mat.Albedo = { 0.8f, 0.8f, 0.8f };
		mat.Roughness = 0.5;
		mat.Metallic = 0;
		Materials.push_back(mat);
	}

};