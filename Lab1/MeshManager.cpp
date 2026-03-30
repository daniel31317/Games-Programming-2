#include "MeshManager.h"
#include <thread>

MeshManager::MeshManager()
{

}

void MeshManager::LoadMeshes()
{
	
	m_meshList[MONKEY] = std::make_unique<Mesh>();
	m_meshList[AMX_50] = std::make_unique<Mesh>();
	m_meshList[LECLERC] = std::make_unique<Mesh>();

	std::thread thread1{ &Mesh::loadModel, m_meshList[MONKEY].get(), "..\\res\\monkey3.obj" };
	std::thread thread2{ &Mesh::loadModel, m_meshList[AMX_50].get(), "..\\res\\AMX-50.obj" };
	std::thread thread3{ &Mesh::loadModel, m_meshList[LECLERC].get(), "..\\res\\Leclerc_AZUR.obj" };

	thread1.join();
	thread2.join();
	thread3.join();

	m_meshList[MONKEY]->uploadModelToGPU();
	m_meshList[AMX_50]->uploadModelToGPU();
	m_meshList[LECLERC]->uploadModelToGPU();


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
	m_meshList[QUAD]->uploadModelToGPU();
}

Mesh* MeshManager::GetMesh(MeshName name)
{
	return m_meshList[name].get();
}