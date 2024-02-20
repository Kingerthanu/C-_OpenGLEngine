#include "projectile.h"

Projectile::Projectile(std::vector<Vertex>& lineMeshes, const glm::vec3& origin, const glm::vec3& orientation)
{

	std::vector<GLuint> stepOrder = { 0, 1 };
	std::vector<Texture> texs;	// Placeholder (inferred to use previously enabled texture for rays [insurmountable])

	this->projectileMesh = new Mesh(lineMeshes, stepOrder, texs);
	this->projectileModel = new Model("models/projectile_grenade/scene.gltf", 0);
	this->projectileModel->grabBounds();

	// Set the model matrix for the projectile
	projectileModel->matricesMeshes[0] = glm::inverse(glm::lookAt(projectileMesh->vertices[0].position, projectileMesh->vertices[1].position, glm::vec3(0.0f, 1.0f, 0.0f))) * projectileModel->matricesMeshes[0];
	this->velocity = 9.05f * glm::vec4((projectileMesh->vertices[1].position - projectileMesh->vertices[0].position), 1.0f) * projectileModel->matricesMeshes[0];

	

	//std::cout << "Projectile vertex amounts " << this->projectileModel->meshes[0].vertices.size() << "\n\n\n\n\n\n";

}

bool Projectile::checkCollisions(Entity**& enemyBatch)
{
	// Extract positions from model matrices
	glm::vec3 projecMatPos = glm::vec3(this->projectileModel->matricesMeshes[0][3]);

	// Calculate min and max points for the bounding boxes
	glm::vec3 projecPosMin = projecMatPos + this->projectileModel->minB;
	glm::vec3 projecPosMax = projecMatPos + this->projectileModel->maxB;
	glm::vec3 enemyMatPos;
	glm::vec3 enemyPosMin;
	glm::vec3 enemyPosMax;

	for (int i = 0; i < 10; i++) {
		if (!enemyBatch[i]) {
			continue;
		}

		
		enemyMatPos = glm::vec3(enemyBatch[i]->entityModel->matricesMeshes[0][3]);
		enemyPosMin = enemyMatPos + enemyBatch[i]->minB;
		enemyPosMax = enemyMatPos + enemyBatch[i]->maxB;

		// Check for non-collision along each axis
		if (!(projecPosMax.x < enemyPosMin.x) && !(projecPosMin.x > enemyPosMax.x) &&
			!(projecPosMax.y < enemyPosMin.y) && !(projecPosMin.y > enemyPosMax.y) &&
			!(projecPosMax.z < enemyPosMin.z) && !(projecPosMin.z > enemyPosMax.z)) {
			// No collision on any axis, continue to the next enemy
			delete enemyBatch[i];
			enemyBatch[i] = nullptr;

			return true;
		}

		// Collision detected, handle it
		

	}

}

void Projectile::shift(Shader& curShader, Camera& playerCam)
{

	this->projectileModel->shift(this->velocity);
	this->projectileModel->Draw(curShader, playerCam);
	this->projectileMesh->drawRay(curShader, playerCam);
	this->velocity[1] -= 0.05f;

}

Projectile::~Projectile()
{

	delete this->projectileModel;

	// Issue was deconstruction order, do inverse. (May want to check why mesh of model is deleting to allow this to be associated in the mesh deconstructor).
	this->projectileMesh->EB1.unBind();
	this->projectileMesh->VB1.unBind();
	this->projectileMesh->VA1.unBind();

	this->projectileMesh->EB1.Delete();
	this->projectileMesh->VB1.Delete();
	this->projectileMesh->VA1.Delete();
	delete this->projectileMesh;



}
