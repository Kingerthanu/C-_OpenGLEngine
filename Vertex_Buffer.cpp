#include "Vertex_Buffer.h"

#include <iostream>

Vertex_Buffer::Vertex_Buffer(std::vector<Vertex>& vertices)
{
	// Create one buffer and store in this
	glGenBuffers(1, &this->ID);

	// Set buffer type which this is bound to our associated (already turned on VAO; gl already knows which one we are talking about)
	glBindBuffer(GL_ARRAY_BUFFER, this->ID); 

	// Feed in data to now attached static variable
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);



}



Vertex_Buffer::Vertex_Buffer()
{

	// Empty debug

}

Vertex_Buffer::~Vertex_Buffer()
{

	//std::cout << "Deconstructing VB...\n";

}

void Vertex_Buffer::Bind()
{
	// Bind to buffer type (bind this to our VAO)
	glBindBuffer(GL_ARRAY_BUFFER, this->ID);

}

void Vertex_Buffer::unBind()
{
	// unBind to buffer type (VAO)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Vertex_Buffer::Delete()
{
	// Remove the one stored buffer in this
	glDeleteBuffers(1, &this->ID);

}