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
	T80HULL_M,
	T80TURRET_M,
	LECLERCHULL_M,
	LECLERCTURRET_M,
	LECLERCBARREL_M,
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

