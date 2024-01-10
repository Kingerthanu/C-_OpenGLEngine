#include "plane.h"

Plane::Plane(const glm::vec3& origin, const glm::vec3& normal)
{

	this->normal = normal;
	this->origin = origin;
	this->equation[0] = normal.x;
	this->equation[1] = normal.y;
	this->equation[2] = normal.z;
	this->equation[3] = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);

};

Plane::Plane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{

	this->normal = glm::normalize(glm::cross((p2 - p1), (p3 - p1)));
	this->origin = (p1+p2+p3) / 3.0f;

	this->equation[0] = normal.x;
	this->equation[1] = normal.y;
	this->equation[2] = normal.z;
	this->equation[3] = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);


};

bool Plane::isFrontFacingTo(const glm::vec3& direction) const
{
	// Check if our direction based on the planes normal is in range (low value as 0 isn't really possible normally)
	float dot = glm::dot(normal, direction);
	return (dot <= 0.01f);

};

double Plane::signedDistanceTo(const glm::vec3& point) const
{

	// Distance from current user ellipsoid
	return (glm::dot(point, this->normal) + this->equation[3]);

};