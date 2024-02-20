#ifndef PLAYER_H
#define PLAYER_H

#include "Collision_Package.h"
#include "projectile.h"


class Player {

	public:

		Camera* playerCam = nullptr;
		Model* tstModel = nullptr;
		Projectile* bulletBin = nullptr;
		glm::vec3* trajectory = nullptr;

		int binSize = 0;

		bool fire_debug = false;

		short unsigned int collisionRecursiveDepth = 0;

		CollisionPacket* collisionPackage = new CollisionPacket();

		Player(const int& width, const int& height, const glm::vec3& pos, Model& tstMod);

		void inputs(GLFWwindow*& window, Shader current); // New silly add with shader for debug on bullet caste
		void collideAndSlide(const glm::vec3& velocity, const glm::vec3& gravity);

		void checkCollision();

		glm::vec3 collideWithWorld(const glm::vec3 ePosition, const glm::vec3 eVelocity);


};

#endif