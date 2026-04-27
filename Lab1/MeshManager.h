#pragma once
#include <memory>
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Mesh.h"


enum MeshName
{
	QUAD,
	LECLERCBODY,
	LECLERCTURRET,
	NUM_MESH
};


class MeshManager
{
private:

	std::unique_ptr<Mesh> m_meshList[NUM_MESH];

public:

	MeshManager();

	void LoadMeshes();

	Mesh* GetMesh(MeshName name);

};

