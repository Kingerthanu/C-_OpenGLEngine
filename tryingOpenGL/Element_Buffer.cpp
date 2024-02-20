#include "Element_Buffer.h"

#include <iostream>

Element_Buffer::Element_Buffer(std::vector<GLuint>& indices)
{
	// Create a single element buffer signed to this object
	glGenBuffers(1, &this->ID);

	// Tell GL that this object will be holding indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);

	// Store set indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);



}

Element_Buffer::~Element_Buffer()
{
	//std::cout << "Deconstructing EB...\n";

}

Element_Buffer::Element_Buffer()
{

	// Empty debug

}

void Element_Buffer::Bind()
{
	// Connect to gl's static element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);

}

void Element_Buffer::unBind()
{
	// Disconnect to gl's static element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Element_Buffer::Delete()
{

	glDeleteBuffers(1, &this->ID);

}