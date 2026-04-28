#include "MeshManager.h"
#include <thread>

MeshManager::MeshManager()
{

}

void MeshManager::LoadMeshes()
{
	
	m_meshList[LECLERCBODY] = std::make_unique<Mesh>();
	m_meshList[LECLERCTURRET] = std::make_unique<Mesh>();
	m_meshList[CITY] = std::make_unique<Mesh>();


	std::thread thread1{ &Mesh::loadModel, m_meshList[LECLERCBODY].get(), "..\\res\\LeclercBody" };
	std::thread thread2{ &Mesh::loadModel, m_meshList[LECLERCTURRET].get(), "..\\res\\LeclercTurret" };
	std::thread thread3{ &Mesh::loadModel, m_meshList[CITY].get(), "..\\res\\City" };

	thread1.join();
	thread2.join();
	thread3.join();

	m_meshList[LECLERCBODY]->uploadModelToGPU();
	m_meshList[LECLERCTURRET]->uploadModelToGPU();
	m_meshList[CITY]->uploadModelToGPU();


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

	m_meshList[CUBE] = std::make_unique<Mesh>();

    Vertex vertices1[] = {
        // Positions              // UVs       // Normals (facing out)
        // Front Face (z = 1)
        Vertex(glm::vec3(-1, -1,  1), glm::vec2(0, 0), glm::vec3(0, 0, 1)), // 0
        Vertex(glm::vec3(1, -1,  1), glm::vec2(1, 0), glm::vec3(0, 0, 1)), // 1
        Vertex(glm::vec3(1,  1,  1), glm::vec2(1, 1), glm::vec3(0, 0, 1)), // 2
        Vertex(glm::vec3(-1,  1,  1), glm::vec2(0, 1), glm::vec3(0, 0, 1)), // 3

        // Back Face (z = -1)
        Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)), // 4
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)), // 5
        Vertex(glm::vec3(-1,  1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)), // 6
        Vertex(glm::vec3(1,  1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)), // 7

        // Left Face (x = -1)
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)), // 8
        Vertex(glm::vec3(-1, -1,  1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)), // 9
        Vertex(glm::vec3(-1,  1,  1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)), // 10
        Vertex(glm::vec3(-1,  1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)), // 11

        // Right Face (x = 1)
        Vertex(glm::vec3(1, -1,  1), glm::vec2(0, 0), glm::vec3(1, 0, 0)),  // 12
        Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(1, 0, 0)),  // 13
        Vertex(glm::vec3(1,  1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)),  // 14
        Vertex(glm::vec3(1,  1,  1), glm::vec2(0, 1), glm::vec3(1, 0, 0)),  // 15

        // Top Face (y = 1)
        Vertex(glm::vec3(-1,  1,  1), glm::vec2(0, 0), glm::vec3(0, 1, 0)),  // 16
        Vertex(glm::vec3(1,  1,  1), glm::vec2(1, 0), glm::vec3(0, 1, 0)),  // 17
        Vertex(glm::vec3(1,  1, -1), glm::vec2(1, 1), glm::vec3(0, 1, 0)),  // 18
        Vertex(glm::vec3(-1,  1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)),  // 19

        // Bottom Face (y = -1)
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)), // 20
        Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, -1, 0)), // 21
        Vertex(glm::vec3(1, -1,  1), glm::vec2(1, 1), glm::vec3(0, -1, 0)), // 22
        Vertex(glm::vec3(-1, -1,  1), glm::vec2(0, 1), glm::vec3(0, -1, 0))  // 23
    };

    unsigned int indices1[] = {
        0, 1, 2,   2, 3, 0,    // Front
        4, 5, 6,   6, 7, 4,    // Back
        8, 9, 10,  10, 11, 8,  // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Top
        20, 21, 22, 22, 23, 20  // Bottom
    };

    unsigned int numVertices1 = sizeof(vertices1) / sizeof(vertices1[0]);

	unsigned int numIndices1 = sizeof(indices1) / sizeof(indices1[0]);

	m_meshList[CUBE]->init(vertices1, numVertices1, indices1, numIndices1);
	m_meshList[CUBE]->setDefaultColour(glm::vec3(1, 0, 1)); // Magenta
	m_meshList[CUBE]->uploadModelToGPU();


}

Mesh* MeshManager::GetMesh(MeshName name)
{
	return m_meshList[name].get();
}