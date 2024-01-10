#include "Vertex_Array.h"

#include <iostream>

Vertex_Array::Vertex_Array()
{
	// Create vertex array and store one va in this container
	glGenVertexArrays(1, &this->ID);

}

Vertex_Array::~Vertex_Array()
{

	//std::cout << "Deconstructing VA...\n";

}

void Vertex_Array::LinkAttrib(Vertex_Buffer &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	// Tell the binded Buffer already in our VAO to add this 
	//VBO.Bind();

	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);

	glEnableVertexAttribArray(layout);

	//VBO.unBind();

}

void Vertex_Array::Bind()
{

	// Activate given vert array in use for gl
	glBindVertexArray(this->ID);

}

void Vertex_Array::unBind()
{
	// Activate nothing burger in use for gl
	glBindVertexArray(0);

}

void Vertex_Array::Delete()
{

	// Clear stored vertex arrays (only one for now) stored in this container
	glDeleteVertexArrays(1, &this->ID);

}