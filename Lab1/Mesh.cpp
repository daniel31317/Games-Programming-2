#include "Mesh.h"
#include <vector>
#include <fstream>
#include <iostream>


Mesh::Mesh()
{

}


Mesh::~Mesh()
{
	
}

// Clean up the VAO and VBOs called by mesh manager destructor instead of normal destructor to allow for deleteing of game objects in real time
void Mesh::unload()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
		model.indices.push_back(indices[i]);
}

void Mesh::loadModel(const std::string& filename)
{
	std::string binPath = filename + ".bin";

	std::ifstream f(binPath.c_str());

	if (f.good())
	{
		std::cout << "Loading Mesh - " + filename + ".bin\n";
		loadModelAsBinary(binPath);
		std::cout << "Finished Loading Mesh - " + filename + ".bin\n";
	}
	else
	{
		std::cout << "Loading Mesh - " + filename + ".obj\n";
		model = OBJModel(filename + ".obj", defaultColour).ToIndexedModel();
		saveModelAsBinary(binPath);
		std::cout << "Finished Loading Mesh - " + filename + ".obj\n";
	}
	
}



//i made it so it saves models to a binary file so load time is shorter because it was taking a while to load and getting annoying to debug
//this unironically saved me hours of waiting based on how many times i played the game just to change one value for it to be wrong
void Mesh::saveModelAsBinary(const std::string& filename)
{
	std::ofstream file(filename, std::ios::binary);

	uint32_t posCount = model.positions.size();
	uint32_t texCount = model.texCoords.size();
	uint32_t normCount = model.normals.size();
	uint32_t colCount = model.colours.size();
	uint32_t idxCount = model.indices.size();

	file.write(reinterpret_cast<char*>(&posCount), sizeof(uint32_t));
	file.write(reinterpret_cast<char*>(&texCount), sizeof(uint32_t));
	file.write(reinterpret_cast<char*>(&normCount), sizeof(uint32_t));
	file.write(reinterpret_cast<char*>(&colCount), sizeof(uint32_t));
	file.write(reinterpret_cast<char*>(&idxCount), sizeof(uint32_t));

	file.write(reinterpret_cast<char*>(model.positions.data()), posCount * sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(model.texCoords.data()), texCount * sizeof(glm::vec2));
	file.write(reinterpret_cast<char*>(model.normals.data()), normCount * sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(model.colours.data()), colCount * sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(model.indices.data()), idxCount * sizeof(uint32_t));
}

void Mesh::loadModelAsBinary(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);

	uint32_t posCount, texCount, normCount, idxCount, colCount;
	file.read(reinterpret_cast<char*>(&posCount), sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(&texCount), sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(&normCount), sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(&colCount), sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(&idxCount), sizeof(uint32_t));

	// Resize and read directly into vectors
	model.positions.resize(posCount);
	model.texCoords.resize(texCount);
	model.normals.resize(normCount);
	model.colours.resize(colCount);
	model.indices.resize(idxCount);

	file.read(reinterpret_cast<char*>(model.positions.data()), posCount * sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(model.texCoords.data()), texCount * sizeof(glm::vec2));
	file.read(reinterpret_cast<char*>(model.normals.data()), normCount * sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(model.colours.data()), colCount * sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(model.indices.data()), idxCount * sizeof(uint32_t));
}




void Mesh::uploadModelToGPU()
{
	drawCount = model.indices.size();

	glGenVertexArrays(1, &vertexArrayObject); //generate a vertex array and store it in the VAO
	glBindVertexArray(vertexArrayObject); //bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)

	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers); //generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VERTEXBUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (model.colours.empty())
	{
		//fill with white if no colours are specified
		std::vector<glm::vec3> defaultColors(model.positions.size(), glm::vec3(1.0f, 1.0f, 1.0f));
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
		glBufferData(GL_ARRAY_BUFFER, defaultColors.size() * sizeof(defaultColors[0]), &defaultColors[0], GL_STATIC_DRAW);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[COLOR_VB]);
		glBufferData(GL_ARRAY_BUFFER, model.colours.size() * sizeof(model.colours[0]), &model.colours[0], GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	
	glBindVertexArray(0); // unbind our VAO

}



void Mesh::draw()
{
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



