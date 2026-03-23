#include "MeshManager.h"

MeshManager::MeshManager()
{

}

void MeshManager::LoadMeshes()
{
	m_meshList[MONKEY] = std::make_unique<Mesh>();
	m_meshList[MONKEY]->loadModel("..\\res\\monkey3.obj");
	m_meshList[AMX_50] = std::make_unique<Mesh>();
	m_meshList[AMX_50]->loadModel("..\\res\\AMX-50.obj");
	m_meshList[LECLERC] = std::make_unique<Mesh>();
	m_meshList[LECLERC]->loadModel("..\\res\\Leclerc_AZUR.obj");

	m_meshList[QUAD] = std::make_unique<Mesh>();

	Vertex vertices[] = { Vertex(glm::vec3(-1.0, 1.0, 0), glm::vec2(0.0, 1.0), glm::vec3(0,0,1)),
					Vertex(glm::vec3(1.0, 1.0, 0), glm::vec2(1.0, 1.0), glm::vec3(0,0,1)),
					Vertex(glm::vec3(-1.0, -1.0, 0), glm::vec2(0.0, 0.0), glm::vec3(0,0,1)),
					Vertex(glm::vec3(-1.0, -1.0, 0), glm::vec2(0.0, 0.0), glm::vec3(0,0,1)),
					Vertex(glm::vec3(1.0, 1.0, 0), glm::vec2(1.0, 1.0), glm::vec3(0,0,1)),
					Vertex(glm::vec3(1.0, -1.0, 0), glm::vec2(1.0, 0.0), glm::vec3(0,0,1)) };

	unsigned int numVertices = sizeof(vertices) / sizeof(vertices[0]);

	unsigned int indices[] = { 0, 1, 2, 3, 4, 5 };
	unsigned int numIndices = sizeof(indices) / sizeof(indices[0]);

	m_meshList[QUAD]->init(vertices, numVertices, indices, numIndices);
}

Mesh* MeshManager::GetMesh(MeshName name)
{
	return m_meshList[name].get();
}