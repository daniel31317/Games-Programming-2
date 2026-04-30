#include "MeshManager.h"
#include <thread>

MeshManager::MeshManager()
{

}
MeshManager::~MeshManager()
{
    for(int i = 0; i < NUM_MESH; i++)
    {
        m_meshList[i]->unload();
	}
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
        // Front Face (z = 0.5)
        Vertex(glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 0), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
        // Back Face (z = -0.5)
        Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
        Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
        Vertex(glm::vec3(-0.5,  0.5, -0.5), glm::vec2(1, 1), glm::vec3(0, 0, -1)),
        Vertex(glm::vec3(0.5,  0.5, -0.5), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
        // Left Face (x = -0.5)
        Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
        Vertex(glm::vec3(-0.5, -0.5,  0.5), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
        Vertex(glm::vec3(-0.5,  0.5,  0.5), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
        Vertex(glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),
        // Right Face (x = 0.5)
        Vertex(glm::vec3(0.5, -0.5,  0.5), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
        Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
        Vertex(glm::vec3(0.5,  0.5, -0.5), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
        Vertex(glm::vec3(0.5,  0.5,  0.5), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
        // Top Face (y = 0.5)
        Vertex(glm::vec3(-0.5,  0.5,  0.5), glm::vec2(0, 0), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(0.5,  0.5,  0.5), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(0.5,  0.5, -0.5), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(-0.5,  0.5, -0.5), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
        // Bottom Face (y = -0.5)
        Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0), glm::vec3(0, -1, 0)),
        Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
        Vertex(glm::vec3(0.5, -0.5,  0.5), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
        Vertex(glm::vec3(-0.5, -0.5,  0.5), glm::vec2(0, 1), glm::vec3(0, -1, 0))
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
	m_meshList[CUBE]->uploadModelToGPU();




    m_meshList[CROSSHAIR] = std::make_unique<Mesh>();

    float width = 0.036f; 
    float height = 0.064f; 

    float thicknessX = 0.001f; 
    float thicknessY = 0.003f; 

    Vertex crosshairVertices[] = 
    {

        Vertex(glm::vec3(-width, -thicknessY, 0.0f), glm::vec2(0,0), glm::vec3(0,0,1)),
        Vertex(glm::vec3(width, -thicknessY, 0.0f), glm::vec2(1,0), glm::vec3(0,0,1)),
        Vertex(glm::vec3(width,  thicknessY, 0.0f), glm::vec2(1,1), glm::vec3(0,0,1)),
        Vertex(glm::vec3(-width,  thicknessY, 0.0f), glm::vec2(0,1), glm::vec3(0,0,1)),

        Vertex(glm::vec3(-thicknessX, -height, 0.0f), glm::vec2(0,0), glm::vec3(0,0,1)),
        Vertex(glm::vec3(thicknessX, -height, 0.0f), glm::vec2(1,0), glm::vec3(0,0,1)),
        Vertex(glm::vec3(thicknessX,  height, 0.0f), glm::vec2(1,1), glm::vec3(0,0,1)),
        Vertex(glm::vec3(-thicknessX,  height, 0.0f), glm::vec2(0,1), glm::vec3(0,0,1))
    };
    unsigned int crosshairIndices[] = {
        0, 1, 2,  2, 3, 0,
        4, 5, 6,  6, 7, 4 
    };

    unsigned int numVertices2 = sizeof(crosshairVertices) / sizeof(crosshairVertices[0]);

    unsigned int numIndices2 = sizeof(crosshairIndices) / sizeof(crosshairIndices[0]);

    m_meshList[CROSSHAIR]->init(crosshairVertices, numVertices2, crosshairIndices, numIndices2);
    m_meshList[CROSSHAIR]->uploadModelToGPU();

}

Mesh* MeshManager::GetMesh(MeshName name)
{
	return m_meshList[name].get();
}