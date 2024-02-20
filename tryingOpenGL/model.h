#ifndef MODEL_H
#define MODEL_H

#include <json/json.hpp>
#include <random>
#include "mesh.h"


using json = nlohmann::json;

class Model
{

public:
	// Take in the file which .gltf file is (perhaps patch for model index being fed in)
	Model(const char* file, const int& ind = 0);
	~Model();
	void grabBounds();
	// Using shader and based on camera draw model
	void Draw(Shader& shader, Camera& camera);
	const std::vector<Mesh>& giveMeshes();

	void genWalls(const int& floorMeshInd);
	void shift(const glm::vec3& translDirection);


	// All meshes and their scalars in own vectors			May want to go back to private after debugging
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;
	glm::vec3 minB, maxB;



private:
	
	// Directory of model
	const char* file;

	// The text of the file
	std::vector<unsigned char> data;

	// The main JSON of model
	json JSON;

	

	// Load in a mesh to the model
	void loadMesh(unsigned int indMesh);

	// Branch through all parts of model and get their parts
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// The 
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTexs;



	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
	std::vector<Texture> getTextures();

	std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);
};

#endif