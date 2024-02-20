#ifndef MESH_CLASS
#define MESH_CLASS

#include <string>

#include "Vertex_Array.h"
#include "Element_Buffer.h"
#include "camera.h"
#include "texture.h"



class Mesh
{

	public:
		// Take in its three parts
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
		Mesh();
		~Mesh();

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		Vertex_Array VA1;
		Vertex_Buffer VB1; // Auto-binded in constructor
		Element_Buffer EB1; // Auto-binded in constructor
		

		// Draw on shader based on camera, with given scalars
		void draw
		(
			Shader& shader, 
			Camera& camera,
			glm::mat4 matrix = glm::mat4(1.0f)
			
		);

		void drawRay	// New silly add
		(
			Shader& shader,
			Camera& camera,
			glm::mat4 matrix = glm::mat4(1.0f)
			
		);

		


};


#endif