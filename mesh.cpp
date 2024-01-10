#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	// Set this givens mesh's pieces
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	this->VA1.Bind();

	this->VB1 = Vertex_Buffer(vertices); // Auto-binded in constructor
	this->EB1 = Element_Buffer(indices); // Auto-binded in constructor

	// Link all these traits to its own vertex array handler
	VA1.LinkAttrib(VB1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // Explain where GL can find its indexes
	VA1.LinkAttrib(VB1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // Explain where GL can find its colors
	VA1.LinkAttrib(VB1, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));	// Explain where GL can find origin for texture mapping
	VA1.LinkAttrib(VB1, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));	// Explain where GL can find origin for texture mapping

	// Unbind will make sure GL unplugs connections with itself so its not messed with
	this->VA1.unBind();
	this->VB1.unBind();
	this->EB1.unBind();

}

Mesh::Mesh()
{

	// Empty construct for debug

}

// Using our shader and the screens relative position use the given matrix rotations to place the given bundle of points
void Mesh::drawRay
(
	Shader& shader,
	Camera& camera,
	glm::mat4 matrix
)
{
	// Turn on shader and ping to GL or vertex's
	shader.Activate();
	VA1.Bind();

	// Tell shader where the user is before telling to draw elements of mesh
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	// Send them to the shader prog
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));


	// Tell shader prog to draw the specified points
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

	VA1.unBind();

}

// Using our shader and the screens relative position use the given matrix rotations to place the given bundle of points
void Mesh::draw
(
	Shader& shader, 
	Camera& camera,
	glm::mat4 matrix

)
{
	// Turn on shader and ping to GL or vertex's
	shader.Activate();
	VA1.Bind();
	
	

	// Count up our texture types for easier reading in our texture container
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	//std::cout << "TEXTURE SIZE -- " << textures.size() << '\n';

	// Check how many diffuse and specular objects we need to shade
	for (unsigned int i = 0; i < textures.size(); i++) {

		std::string num;
		std::string type = textures[i].type;

		if (type == "diffuse") {

			num = std::to_string(numDiffuse++);

		}
		else if (type == "specular") {

			num = std::to_string(numSpecular++);
		}

		//std::cout << "UNIFORM ----- " << (type + num).c_str() << '\n';

		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Tell shader where the user is before telling to draw elements of mesh
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	// Get a variable in order to hold new transformation
	glm::mat4 transl = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 scal = glm::mat4(1.0f);

	// Send them to the shader prog
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(transl));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(scal));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));


	// Tell shader prog to draw the specified points
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	this->VA1.unBind();

}

Mesh::~Mesh()
{
	//std::cout << "Deleting Mesh...\n\n";
	
}