#include "entity.h"

Entity::Entity(const char* file, const int& ind, const glm::vec3& offset, const float& speed)
{
	
	this->entityModel = new Model(file, ind);

	this->entityModel->shift(glm::vec3(offset[0], offset[1], offset[2]));


	//std::cout << "\n\n\n\n\n\n VERTICES " << this->entityModel->meshes[0].vertices.size() << "\n\n";
	this->entityCenter = this->entityModel->meshes[0].vertices[34].position;

	this->minB = this->entityModel->meshes[0].vertices[40].position;
	this->maxB = this->entityModel->meshes[0].vertices[36].position;
	this->speed = speed;

}

bool flag = false;



void Entity::react(Shader& curShader, Camera& playerCam)
{
	
	velocity += ((0.000035f * ((speed * (playerCam.position - glm::vec3(this->entityModel->matricesMeshes[0] * glm::vec4(this->entityCenter, 1.0f))) - velocity))) - 0.01f * velocity);
	velocity[1] = 0.0f;

	this->entityModel->shift(velocity);

	this->entityModel->Draw(curShader, playerCam);

}

Entity::~Entity()
{
	
	delete this->entityModel;

}