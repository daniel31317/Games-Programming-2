#include "Mesh.h"
#include <vector>


Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
	drawCount = numVertices;

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	std::vector<glm::vec3> positions; //holds the position data
	std::vector<glm::vec2> textCoords; //holds the texture coordinate data

	// reserve the all the space needed to hold our data
	positions.reserve(numVertices);
	textCoords.reserve(numVertices);

	for (int i = 0; i < numVertices; i++)
	{
		positions.push_back(vertices[i].pos); //store our array of vertex positon into a list vec3 positions

		textCoords.push_back(vertices[i].texCoord);

	}

	

	 //generate our buffers based of our array of data / buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);

	//tell opengl what type of data the buffer is(GL_ARRAY_BUFFER), and pass the data
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VERTEXBUFFER]); 

	//move the data to the GPU - type of data, size of data, starting address (pointer)of data, where do we store the data on the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	

	//tell opengl how many attributes there are in out object
	glEnableVertexAttribArray(0);
	//tell open gl to read the vertices array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//tell opengl what type of data the buffer is(GL_ARRAY_BUFFER), and pass the data
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]); 
	//move the data to the GPU - type of data, size of data, starting address (pointer)of data, where do we store the data on the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(textCoords[0]), &textCoords[0],GL_STATIC_DRAW); 

	//tell opengl how many attributes there are in out object
	glEnableVertexAttribArray(1);
	//tell open gl to read the vertices array
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0); // unbind our VAO



}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::Draw()
{
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, drawCount);
	glBindVertexArray(0);

}