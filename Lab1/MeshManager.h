#pragma once
#include <memory>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Mesh.h"


enum MeshName
{
	QUAD_M,
	CUBE_M,
	CROSSHAIR_M,

	//https://sketchfab.com/3d-models/tank-t-80u-ebf4b55eeabb421cbf2758a2ec948439#download
	T80HULL_M,
	T80TURRET_M,

	//https://sketchfab.com/3d-models/char-leclerc-84a0918d2f534c2eb003ab3cb3029c03#download
	LECLERCHULL_M,
	LECLERCTURRET_M,
	LECLERCBARREL_M,

	//https://free3d.com/3d-model/huge-city-788526.html?dd_referrer=
	CITY_M,
	NUM_MESH
};


class MeshManager
{
private:

	std::unique_ptr<Mesh> m_meshList[NUM_MESH];

public:

	MeshManager();
	~MeshManager();

	void LoadMeshes();

	Mesh* GetMesh(MeshName name);

};

