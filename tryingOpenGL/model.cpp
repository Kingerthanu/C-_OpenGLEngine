#include "model.h"

Model::~Model() 
{

	// Go through each mesh and call its draw func
	for (unsigned int i = 0; i < meshes.size(); i++) {

		//std::cout << i << "st Clear\n";

		if (i < meshes[i].textures.size()) {

			meshes[i].textures[i].Unbind();
			meshes[i].textures[i].Delete();

		}

		//																															May actually already be converted into right form dont need , translationsMeshes[i], rotationsMeshes[i], scalesMeshes[i]
		// Issue was deconstruction order, do inverse. (May want to check why mesh of model is deleting to allow this to be associated in the mesh deconstructor).
	
		meshes[i].EB1.unBind();
		meshes[i].VB1.unBind();
		meshes[i].VA1.unBind();

		//std::cout << "Unbinded\n";

		
		meshes[i].EB1.Delete();
		meshes[i].VB1.Delete();
		meshes[i].VA1.Delete();
		
		

		//std::cout << "Deleted\n";

		

	}

}


// May want to in future seperate this function from all models
void Model::genWalls(const int& floorMeshInd)
{

	// Get random coord in general range
	unsigned int size = this->meshes[floorMeshInd].vertices.size();
	float xVal, yVal, xValOff, yValOff;

	std::random_device rand;   // Making seeded gen
	std::mt19937 seed(rand()); // Seeding with quicker gen with seed of rand gen
	std::uniform_real_distribution<float> randX(-1, 1);
	std::uniform_real_distribution<float> randZ(-1, 1);

	for (int i = 0; i < 10; i++) {

		randX = std::uniform_real_distribution<float>(-1, 1);
		randZ = std::uniform_real_distribution<float>(-1, 1);

		xVal = randX(seed);
		yVal = randZ(seed);
		
		randX = std::uniform_real_distribution<float>(-0.2, 0.2);
		randZ = std::uniform_real_distribution<float>(-0.2, 0.2);

		xValOff = randX(seed);
		yValOff = randZ(seed);
		//std::cout << "SIZE - " << this->meshes[1].indices.size() << '\n';

		glm::vec3 cornerOne = glm::vec3(xVal, this->meshes[floorMeshInd].vertices[0].position.y, yVal);
		glm::vec3 cornerTwo = glm::vec3(xVal + xValOff, this->meshes[floorMeshInd].vertices[0].position.y, yVal + yValOff);
		glm::vec3 topCornerOne = glm::vec3(xVal, this->meshes[floorMeshInd].vertices[0].position.y + 1.05f, yVal);
		glm::vec3 topCornerTwo = glm::vec3(xVal + xValOff, this->meshes[floorMeshInd].vertices[0].position.y + 1.05f, yVal + yValOff);
		glm::vec3 normal = (glm::normalize(glm::cross((topCornerTwo - cornerOne), (cornerTwo - topCornerOne))));
		glm::vec3 spacer = glm::vec3(0.0001f, 0.0001f, 0.0001f);

		//std::cout << "Normal val - " << normal.x << ", " << normal.y << ", " << normal.z << "\n";

		if (normal.x + normal.z < 0) {
			
			spacer *= -1;
			//std::cout << "Inverting...\n";

		}

		// Normal error stemmed from winding order of inserted vertexes. BackSide should be opposite order with its negative normal. Spacer if being added and its parts sum to a positive, can just be its positive spacer; but if sum is negative, need to negate the spacer or will have inward pointing normals

		this->meshes[floorMeshInd].vertices.insert(this->meshes[floorMeshInd].vertices.end(), {
			Vertex{cornerOne, normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV}, Vertex{cornerTwo, normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV},
			Vertex{topCornerOne, normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV}, Vertex{ topCornerTwo, normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV },
			Vertex{cornerTwo + spacer, -normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV}, Vertex{cornerOne + spacer, -normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV},
			Vertex{ topCornerTwo + spacer, -normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV }, Vertex{topCornerOne + spacer, -normal, this->meshes[floorMeshInd].vertices[0].color, this->meshes[floorMeshInd].vertices[0].texUV} });

		this->meshes[floorMeshInd].indices.insert(this->meshes[floorMeshInd].indices.end(), { size, size+1, size+2, size+1, size+3, size+2, size+4, size + 5, size + 6, size + 5, size + 7, size + 6 });

		size = this->meshes[floorMeshInd].vertices.size();

	}

	this->meshes[floorMeshInd].VB1.Bind();
	this->meshes[floorMeshInd].EB1.Bind();

	glBufferData(GL_ARRAY_BUFFER, this->meshes[floorMeshInd].vertices.size() * sizeof(Vertex), this->meshes[floorMeshInd].vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshes[floorMeshInd].indices.size() * sizeof(GLuint), this->meshes[floorMeshInd].indices.data(), GL_STATIC_DRAW);

	this->meshes[floorMeshInd].VB1.unBind();
	this->meshes[floorMeshInd].EB1.unBind();

	//std::cout << "Corner One Coords: " << cornerOne.x << ' ' << cornerOne.y << ' ' << cornerOne.z << " | Corner Two Coords: " << cornerTwo.x << ' ' << cornerTwo.y << ' ' << cornerTwo.z << "\n\n";

}

void Model::grabBounds()
{
	json accessors = this->JSON["accessors"];
	glm::vec3 val;

	for (auto& accessor : accessors) {

		if (accessor.find("max") != accessor.end()) {

			// Access "max" from the current accessor, not from the entire set of accessors
			val = glm::vec3((float)accessor["max"][0], (float)accessor["max"][1], (float)accessor["max"][2]);

			if (this->maxB.x < val.x && this->maxB.y < val.y && this->maxB.z < val.z) {
				this->maxB = val;
			}

			val = glm::vec3((float)accessor["min"][0], (float)accessor["min"][1], (float)accessor["min"][2]);

			if (this->minB.x > val.x && this->minB.y > val.y && this->minB.z > val.z) {
				this->minB = val;
			}

		}
	}

}

Model::Model(const char* file, const int& ind)
{

	// Get the models position
	std::string text = get_file_contents(file);
	
	// Interpret this as a json file
	this->JSON = json::parse(text);

	// Set this models file to given and get data from json and store in data
	this->file = file; // changed from ::
	this->data = getData();
	// Grab all parts of model for drawing / connecting
	traverseNode(ind);
	
}

void Model::loadMesh(unsigned int indMesh)
{

	// Get all positions of parts in mesh
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	//std::cout << "INSIDE LOAD M FINISHED INDEXERS" << '\n';

	std::vector<float> positionVec = getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(positionVec);

	//std::cout << 'P' << '\n';

	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);

	//std::cout << 'N' << '\n';

	//std::cout << "TEXTURES INDEXES BEFORE SEARCH -- " << texAccInd  << '\n';

	std::vector<float> textureVec = getFloats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(textureVec);				
	
	//std::cout << "\n\nTEXTURES UV " << texUVs[0][0] << ' ' << texUVs[0][1] << " TEXTURE 2 " << texUVs[1][0] << ' ' << texUVs[1][1] << '\n';

	//std::cout << 'T' << '\n';

	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);

	//std::cout << 'V' << '\n';

	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);

	//std::cout << 'I' << '\n';

	std::vector<Texture> textures = getTextures();			// Gotta be this somehow

	// Add this mesh to the container of all other meshes of model
	meshes.push_back(Mesh(vertices, indices, textures));

	//std::cout << "MeshingHeimer\n";

}

const std::vector<Mesh>& Model::giveMeshes()
{

	return this->meshes;

}

void Model::Draw(Shader& shader, Camera& camera)
{

	// Go through each mesh and call its draw func
	for (unsigned int i = 0; i < meshes.size(); i++) {

		//																															May actually already be converted into right form dont need , translationsMeshes[i], rotationsMeshes[i], scalesMeshes[i]
		meshes[i].draw(shader, camera, matricesMeshes[i]);

	}

}


std::vector<unsigned char> Model::getData()
{

	std::string bytesStr;
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesStr = get_file_contents((fileDirectory + uri).c_str());

	std::vector<unsigned char> data(bytesStr.begin(), bytesStr.end());
	return data;

}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{

	// Get this json node
	json node = JSON["nodes"][nextNode];

	//std::cout << "NODE -- " << node << '\n';

	// Check for translation part of this given node / mesh
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

	if (node.find("translation") != node.end()) {

		float transValues[3];

		for (unsigned int i = 0; i < node["translation"].size(); i++) {

			transValues[i] = (node["translation"][i]);

		}

		translation = glm::make_vec3(transValues);
		//std::cout << "found" << '\n';
	}

	//std::cout << 'T' << '\n';

	// Check for rotation part of this given node / mesh
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	if (node.find("rotation") != node.end()) {

		float rotValues[4] =
		{

			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]

		};

		rotation = glm::make_quat(rotValues);
		//std::cout << "found" << '\n';
	}

	// Check for scale part of this given node / mesh
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	if (node.find("scale") != node.end()) {

		float scaleValues[3];

		for (unsigned int i = 0; i < node["scale"].size(); i++) {

			scaleValues[i] = (node["scale"][i]);

		}
		//std::cout << "\n\n\n\n\n\n\n\nfound Scalar\n\n\n\n\n\n\n\n" << '\n';
		scale = glm::make_vec3(scaleValues);

	}

	//std::cout << 'S' << '\n';

	// Check for matrix position part of this given node / mesh
	glm::mat4 matNode = glm::mat4(1.0f);

	if (node.find("matrix") != node.end()) {

		float matValues[16];

		for (unsigned int i = 0; i < node["matrix"].size(); i++) {

			matValues[i] = (node["matrix"][i]);

		}

		matNode = glm::make_mat4(matValues);

		//std::cout << "found mat" << '\n';

	}

	//std::cout << 'M' << '\n';

	// Using all new given points adjust some mat4's accordingly
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Multiply matrices together to get current mesh orientation
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	//std::cout << "B4 M" << '\n';

	// If this is a given proper mesh, add it to our model
	if (node.find("mesh") != node.end()) {

		//std::cout << "I M" << '\n';

		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		//std::cout << "PINING NODE MESH BEFORE LOAD -- " << node["mesh"] << '\n';

		// Load mesh's contents now and not just position vectors
		loadMesh(node["mesh"]);

	}

	//std::cout << "A M" << '\n';

	// Recursively go through each child and its nodes
	if (node.find("children") != node.end()) {

		for (unsigned int i = 0; i < node["children"].size(); i++) {

			//std::cout << "\n\nCHECKING CHILD NODE\n\n\n";

			// based on the previous meshes spot, add this new one
			traverseNode(node["children"][i], matNextNode);

		}

	}

	//std::cout << "A C" << '\n';

}

void Model::shift(const glm::vec3& translDirection)
{

	for (int i = this->matricesMeshes.size() - 1; i != -1; i--) {

		this->matricesMeshes[i] = glm::translate(this->matricesMeshes[i], translDirection);

	}

}

std::vector<float> Model::getFloats(json accessor)
{

	// Return vec
	std::vector<float> floatVec;

	// Get index offsets of bufferView, the amount of parts, the acc Byte offset for each part, and the type of data we are checking
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;

	// Check type for iterating and grabbing data in next.
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Invalid type ( SCALAR, VEC2, VEC3 )");

	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;

	//std::cout << "BEGINNING OF TEXTURE DATA -- ";

	// Go through and grab binaries of data and interpret as floats
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData;)
	{

		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);

	}

	return floatVec;
		

}

std::vector<GLuint> Model::getIndices(json accessor)
{

	//std::cout << "INSIDE GET INDICES" << '\n';

	std::vector<GLuint> indices;

	// Get offsets of given components of json piece
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	//std::cout << 'P' << '\n';

	json bufferView = JSON["bufferViews"][buffViewInd];

	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	//std::cout << "Z\n";

	unsigned int beginningOfData = byteOffset + accByteOffset;
	
	if (componentType == 5125) { // If unsigned int

		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4;) {

			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);

		}

	}
	else if (componentType == 5123) { // If unsigned short

		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2;) {

			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short int value;
			std::memcpy(&value, bytes, sizeof(unsigned short int));
			
			indices.push_back((GLuint)value);

		}

	}
	else if (componentType == 5122) { // If short

		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2;) {

			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);

		}

	}

	return indices;

}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{

	std::vector<glm::vec2> vectors;

	for (int i = 0; i < floatVec.size();)
	{

		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));

	}

	return vectors;

}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs)
{

	std::vector<Vertex> vertices;

	for (int i = 0; i < positions.size(); i++) {

		vertices.push_back(Vertex{ positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), texUVs[i] });

	}

	return vertices;

}




std::vector<Texture> Model::getTextures()
{

	std::vector<Texture> textures;

	// Grab directory
	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	//std::cout << "IN GET GET TEXTURES IMAGE JSON SIZE -- " << JSON["images"].size() << '\n';

	// For each image of the model
	for (unsigned int i = 0; i < JSON["images"].size(); i++) 
	{

		// get the path of the given texture
		std::string texPath = JSON["images"][i]["uri"];

		//std::cout << "CURRENT TEXTURE PATH " << texPath << "\n\n";

		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++) {

			if (loadedTexName[j] == texPath) {
				//std::cout << "Checking state....... " << texPath << '\n';
				// If already in universal add to given model
				textures.push_back(loadedTexs[j]);
				skip = true;
				break;

			}

		}

		if (!skip) {

			

			// Check if diffuse or specular type
			if (texPath.find("baseColor") != std::string::npos) {
				std::cout << "is base\n";
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTexs.size());
				textures.push_back(diffuse);
				loadedTexs.push_back(diffuse);
				loadedTexName.push_back(texPath);

			}
			else if (texPath.find("metallicRoughness") != std::string::npos) {
				std::cout << "is metallic\n";
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTexs.size());
				textures.push_back(specular);
				loadedTexs.push_back(specular);
				loadedTexName.push_back(texPath);

			}
		}
	}

	return textures;

}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{

	std::vector<glm::vec4> vectors;

	for (int i = 0; i < floatVec.size();)
	{

		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));

	}

	return vectors;

}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{

	std::vector<glm::vec3> vectors;

	for (int i = 0; i < floatVec.size();)
	{

		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));

	}

	return vectors;

}
