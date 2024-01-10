#ifndef ENTITY_H
#define ENTITY_H

#include "model.h"

class Entity {

	private:
		float speed;
		glm::vec3 velocity;

	public:
		Entity(const char* file, const int& ind, const glm::vec3& offset, const float& speed);
		void react(Shader& curShader, Camera& playerCam);
		~Entity();

		glm::vec3 entityCenter, minB, maxB;
		Model* entityModel;


};

#endif
