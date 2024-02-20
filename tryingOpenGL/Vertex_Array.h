#ifndef VA_H
#define VA_H

#include <glad/glad.h>
#include "Vertex_Buffer.h"

class Vertex_Array
{

public:
	GLuint ID;
	Vertex_Array();
	~Vertex_Array();

	void LinkAttrib(Vertex_Buffer &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void unBind();
	void Delete();



};

#endif

