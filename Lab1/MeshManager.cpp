#include "MeshManager.h"

MeshManager::MeshManager()
{

}

void MeshManager::LoadMeshes()
{
	m_meshList[MONKEY] = std::make_unique<Mesh>();
	m_meshList[MONKEY]->loadModel("..\\res\\monkey3.obj");
}

Mesh* MeshManager::GetMesh(MeshName name)
{
	return m_meshList[name].get();
}