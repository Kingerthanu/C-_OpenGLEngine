#ifndef COLLISION_PAC_H
#define COLLISION_PAC_H

#include "plane.h"									// Some problem with plane.h needing to be rooted, model.h doesn't seem to have issue actually
#include "Collision_Packet.h"



// Caste to unsigned int...?
//#define in(a) ((unsigned int&) a)

bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc);


bool getLowestRoot(float a, float b, float c, float maxR, float* root);


void checkTriangle(CollisionPacket* colPackage, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);


#endif