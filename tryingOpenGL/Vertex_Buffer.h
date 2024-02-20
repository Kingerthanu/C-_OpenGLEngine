#ifndef VB_H
#define VB_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "vertex.h"

class Vertex_Buffer
{

public:
	GLuint ID;
	Vertex_Buffer(std::vector<Vertex>& vertices);
	Vertex_Buffer(); // debug
	~Vertex_Buffer();

	void Bind();
	void unBind();
	void Delete();



};

#endif
