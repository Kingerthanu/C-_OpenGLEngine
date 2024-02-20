#ifndef EB_H
#define EB_H

#include <glad/glad.h>
#include <vector>

class Element_Buffer
{

public:
	GLuint ID;
	Element_Buffer(std::vector<GLuint>& indices);
	Element_Buffer(); // debug
	~Element_Buffer();


	void Bind();

	void unBind();

	void Delete();



};

#endif

