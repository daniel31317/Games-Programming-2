#pragma once
#include <memory>
#include "Mesh.h"


enum MeshName
{
	MONKEY,
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

