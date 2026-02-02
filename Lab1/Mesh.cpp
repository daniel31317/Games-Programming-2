#include "Mesh.h"


Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
	drawCount = numVertices;

	//generate one vertex array and set the location where its stored
	glGenVertexArrays(1, &vertexArrayObject);

	//bind the vertex array object
	glBindVertexArray(vertexArrayObject);

	//generate buffers based on our array of data
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	
	//bind the buffer so we can work on the buffer in our specified positions
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VERTEXBUFFER]);

	//move data to the gpu
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	//tell opengl how many attrivutes there are in out object
	glEnableVertexAttribArray(0);
	
	//tell open gl to read the vertices array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//unbind the buffer
	glBindVertexArray(0);
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