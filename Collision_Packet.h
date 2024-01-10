#ifndef COLLISION_PACKET_H
#define COLLISION_PACKET_H

#include <glm/glm.hpp>

class CollisionPacket
{

public:
	glm::vec3 eRadius = glm::vec3(0.5f, 0.5f, 0.5f);		// Ellipsoid radius

	// Information about the move being requested: (In R3 space)
	glm::vec3 R3Velocity;
	glm::vec3 R3Position;

	// Information about the move being requested: (In eSpace)
	glm::vec3 velocity;
	glm::vec3 normalizedVelocity;
	glm::vec3 basePoint;

	// Hit information
	bool foundCollision;
	double nearestDistance;
	glm::vec3 intersectionPoint;


};

#endif