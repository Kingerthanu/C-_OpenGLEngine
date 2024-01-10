#include "Collision_Package.h"
#include <iostream>

// Caste to unsigned int...?
#define in(a) ((unsigned int&) a)

bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc)
{

	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;

	float a = glm::dot(e10, e10);
	float b = glm::dot(e10, e20);
	float c = glm::dot(e20, e20);
	float ac_bb = (a * c) - (b * b);

	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);

	float d = glm::dot(vp, e10);
	float e = glm::dot(vp, e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;

	return ((in(z)& ~(in(x) | in(y))) & 0x80000000);

};


bool getLowestRoot(float a, float b, float c, float maxR, float* root)
{

	// Check if a solution exists
	float determinant = b * b - 4.0f * a * c;

	// If determinant is neg, no solution.
	if (determinant < 0.0f) {
		return false;
	}

	// Calculate the two roots:

	float sqrtD = std::sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);

	// Sort so x1 <= x2

	if (r1 > r2) {
		float tmp = r2;
		r2 = r1;
		r1 = tmp;
	}

	// Get lowest root:
	if (r1 > 0 && r1 < maxR) {

		*root = r1;
		return true;

	}

	// It is possible we want x2 if x1 < 0
	if (r2 > 0 && r2 < maxR) {
		*root = r2;
		return true;
	}

	// No valid solution
	return false;

};


void checkTriangle(CollisionPacket* colPackage, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{

	Plane trianglePlane(p1, p2, p3);
	
	//std::cout << "Point 1 - " << p1.x << ' ' << p1.y << ' ' << p1.z << " Point 2 - " << p2.x << ' ' << p2.y << ' ' << p2.z << " Point 3 - " << p3.x << ' ' << p3.y << ' ' << p3.z << '\n';

	// Is triangle front-facing to package (we only look at front-facing)

	if (trianglePlane.isFrontFacingTo(colPackage->normalizedVelocity)) {

		// Get interval of plane intersection
		double t0, t1;
		bool embeddedInPlane = false;

		// Calculate the signed distance from collision sphere to triangle's plane
		double signedDistToTrianglePlane = trianglePlane.signedDistanceTo(colPackage->basePoint);

		// cache this as going to use for later
		float normalDotVelocity = glm::dot(trianglePlane.normal, colPackage->velocity);

		// if sphere is travelling parallel to plane
		if (normalDotVelocity == 0.0f) {

			if (fabs(signedDistToTrianglePlane) >= 1.0f) {

				// sphere not colliding
				return;

			}
			else {

				// sphere is in plane ( intersects in given range [0 .. 1]
				embeddedInPlane = true;
				t0 = 0.0;
				t1 = 1.0;

			}

		}
		else {

			// Find intersection interval as not is main check case
			t0 = (-1.0 - signedDistToTrianglePlane) / normalDotVelocity;
			t1 = (1.0 - signedDistToTrianglePlane) / normalDotVelocity;

			// swap for t0 > t1
			if (t0 > t1) {

				double temp = t1;
				t1 = t0;
				t0 = temp;

			}

			// Check that there is at least one result in range
			if (t0 > 1.0f || t1 < 0.0f) {

				// Both t values are outsde [0, 1] so no collision
				return;

			}

			// Clamp to range
			if (t0 < 0.0) t0 = 0.0;
			if (t1 < 0.0) t1 = 0.0;
			if (t0 > 1.0) t0 = 1.0;
			if (t1 > 1.0) t1 = 1.0;

		}

		// Now we have time range, between which collision sphere and triangle plane will occur

		glm::vec3 collisionPoint;
		bool foundCollision = false;
		float t = 1.0f;

		//	First check collision inside triangle if this happens it must be at time t0

		if (!embeddedInPlane) {

			glm::vec3 planeIntersectionPoint = (colPackage->basePoint - trianglePlane.normal) + (float)t0 * colPackage->velocity;

			if (checkPointInTriangle(planeIntersectionPoint, p1, p2, p3)) {

				foundCollision = true;
				t = (float)t0;
				collisionPoint = planeIntersectionPoint;

			}

		}


		// If collision wasn't found on the intitial t0 (this will be big surface bumps) we will sweep across uneven terrain
		if (!foundCollision) {

			//std::cout << "STILL CHECKING COLLISIONS\n";

			glm::vec3 velocity = colPackage->velocity;
			glm::vec3 base = colPackage->basePoint;
			float velocitySquareLength = glm::length(velocity) * glm::length(velocity);				/// May be wrong type of squared length
			float a, b, c;
			float newT;

			// For each vertex we check a quadratic ( check against points )
			a = velocitySquareLength;

			//P1
			b = 2.0f * glm::dot(velocity, (base - p1));
			c = (float)std::pow(glm::length(p1 - base), 2) - 1.0;				/// May be wrong type of squared length

			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollision = true;
				collisionPoint = p1;
			}

			//P2
			b = 2.0f * glm::dot(velocity, (base - p2));
			c = (float)std::pow(glm::length(p2 - base), 2) - 1.0;				/// May be wrong type of squared length

			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollision = true;
				collisionPoint = p2;
			}


			//P3
			b = 2.0f * glm::dot(velocity, (base - p3));
			c = (float)std::pow(glm::length(p3 - base), 2) - 1.0;				/// May be wrong type of squared length

			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollision = true;
				collisionPoint = p3;
			}


			// Check points against its edges

			// p1 -> p2:

			glm::vec3 edge = p2 - p1;
			glm::vec3 baseToVertex = p1 - base;
			float edgeSquaredLength = std::pow(glm::length(edge), 2);
			float edgeDotVelocity = glm::dot(edge, velocity);
			float edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			// Calculate params for equatioons
			a = edgeSquaredLength * -velocitySquareLength + edgeDotVelocity * edgeDotVelocity;

			b = edgeSquaredLength * (2 * glm::dot(velocity, baseToVertex)) - 2.0 * edgeDotVelocity * edgeDotBaseToVertex;

			c = edgeSquaredLength * (1 - std::pow(glm::length(baseToVertex), 2)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			// Does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT)) {

				// Check if intersection is within line segment
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;

				if (f >= 0.0 && f <= 1.0) {

					// Intersection took plane within segment!
					t = newT;
					foundCollision = true;
					collisionPoint = p1 + f * edge;

				}

			}

			// p2 -> p3:

			edge = p3 - p2;
			baseToVertex = p2 - base;
			edgeSquaredLength = std::pow(glm::length(edge), 2);
			edgeDotVelocity = glm::dot(edge, velocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			// Calculate params for equatioons
			a = edgeSquaredLength * -velocitySquareLength + edgeDotVelocity * edgeDotVelocity;

			b = edgeSquaredLength * (2 * glm::dot(velocity, baseToVertex)) - 2.0 * edgeDotVelocity * edgeDotBaseToVertex;

			c = edgeSquaredLength * (1 - std::pow(glm::length(baseToVertex), 2)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			// Does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT)) {

				// Check if intersection is within line segment
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;

				if (f >= 0.0 && f <= 1.0) {

					// Intersection took plane within segment!
					t = newT;
					foundCollision = true;
					collisionPoint = p2 + f * edge;

				}

			}


			// p3 -> p1:

			edge = p1 - p3;
			baseToVertex = p3 - base;
			edgeSquaredLength = std::pow(glm::length(edge), 2);
			edgeDotVelocity = glm::dot(edge, velocity);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);

			// Calculate params for equatioons
			a = edgeSquaredLength * -velocitySquareLength + edgeDotVelocity * edgeDotVelocity;

			b = edgeSquaredLength * (2 * glm::dot(velocity, baseToVertex)) - 2.0 * edgeDotVelocity * edgeDotBaseToVertex;

			c = edgeSquaredLength * (1 - std::pow(glm::length(baseToVertex), 2)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			// Does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT)) {

				// Check if intersection is within line segment
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;

				if (f >= 0.0 && f <= 1.0) {

					// Intersection took plane within segment!
					t = newT;
					foundCollision = true;
					collisionPoint = p3 + f * edge;

				}

			}

			

		}

		// Set result:
		if (foundCollision) {

			// Distance to collision 't' is time of collision impact										maybe length of velocity but dont think so
			float distToCollision = t * glm::length(colPackage->velocity);

			// Does this work as closes hit
			// It IS if its the first hit or closest
			if (!colPackage->foundCollision || distToCollision < colPackage->nearestDistance) {

				colPackage->nearestDistance = distToCollision;
				colPackage->intersectionPoint = collisionPoint;
				colPackage->foundCollision = true;

			}

		}

		// Will all work if not a backface :)


	}


	// Add to char cam for response



};
