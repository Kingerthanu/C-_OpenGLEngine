#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "entity.h"		// May want to change this in order to disassoiate a check collision with each individual projectile class

class Projectile 
{

	public:
		void shift(Shader& curShader, Camera& playerCam);
		bool checkCollisions(Entity**& enemyBatch);

		void draw();
		Projectile(std::vector<Vertex>& lineMeshes, const glm::vec3& origin, const glm::vec3& orientation);
		~Projectile();




	private:

		Mesh* projectileMesh = nullptr;
		Model* projectileModel = nullptr;
		glm::vec3 velocity, minB, maxB;

};

#endif
