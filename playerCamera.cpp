#include "playerCamera.h"

Player::Player(const int& width, const int& height, const glm::vec3& pos, Model& tstMod)
{

	this->playerCam = new Camera(width, height, pos);
	this->tstModel = &tstMod;

}

bool airborne = false;
float pulldown = -0.05f;

void Player::inputs(GLFWwindow*& window, Shader current)
{
	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (this->playerCam->firstClick)
		{
			glfwSetCursorPos(window, (this->playerCam->width / 2), (this->playerCam->height / 2));
			this->playerCam->firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = this->playerCam->sensitivity * (float)(mouseY - (this->playerCam->height / 2)) / this->playerCam->height;
		float rotY = this->playerCam->sensitivity * (float)(mouseX - (this->playerCam->width / 2)) / this->playerCam->width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(this->playerCam->orientation, glm::radians(-rotX), glm::normalize(glm::cross(this->playerCam->orientation, this->playerCam->Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, this->playerCam->Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			this->playerCam->orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		this->playerCam->orientation = glm::rotate(this->playerCam->orientation, glm::radians(-rotY), this->playerCam->Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (this->playerCam->width / 2), (this->playerCam->height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		this->playerCam->firstClick = true;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !this->bulletBin)
	{

		glm::vec3 bulletVec = this->playerCam->position;
		glm::vec3 bulletVel = 10.0f * (this->playerCam->orientation);

		std::vector<Vertex> pos = { Vertex{ bulletVec, this->playerCam->orientation, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) }, Vertex{ bulletVec + bulletVel, this->playerCam->orientation , glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) } };
		std::vector<GLuint> stepOrder = { 0, 1 };
		std::vector<Texture> texs;	// Placeholder (inferred to use previously enabled texture for rays [insurmountable])


		// Currently calling individual Lines to be drawn as their own meshes, because they are all associated, we should place them in a VBO in a universal mesh in which can aid runtime
		this->bulletBin = new Projectile(pos, this->playerCam->position, this->playerCam->orientation);

	}

	if (airborne) {
		glm::vec3 prev = this->playerCam->position;
		collideAndSlide(glm::vec3(0.0f), glm::vec3((*this->trajectory)[0] * 0.085f, pulldown, (*this->trajectory)[2] * 0.085f));

		if (prev[1] == this->playerCam->position[1]) {
			airborne = false;
			delete this->trajectory;
		}
		else {
			return;
		}

		
	}

	// Handles key inputs (cringe)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		collideAndSlide(this->playerCam->speed * this->playerCam->orientation, glm::vec3(0, pulldown,0));
	}
	/*
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {


		// Could use for debugging
		

	}
	*/
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		collideAndSlide(this->playerCam->speed * -glm::normalize(glm::cross(this->playerCam->orientation, this->playerCam->Up)), glm::vec3(0, pulldown, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		collideAndSlide(this->playerCam->speed * -this->playerCam->orientation, glm::vec3(0, -0.15f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		collideAndSlide(this->playerCam->speed * glm::normalize(glm::cross(this->playerCam->orientation, this->playerCam->Up)), glm::vec3(0, pulldown, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		airborne = true;
		this->trajectory = new glm::vec3(this->playerCam->orientation);
		collideAndSlide(this->playerCam->speed * this->playerCam->Up * 34.0f, glm::vec3(0, 0.0f, 0));
		
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		collideAndSlide(this->playerCam->speed * -this->playerCam->Up, glm::vec3(0, pulldown, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{

		if (this->bulletBin) {

			delete this->bulletBin;
			this->bulletBin = nullptr;
		}

		this->playerCam->speed = 0.24f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		this->playerCam->speed = 0.08f;
	}

}

void Player::collideAndSlide(const glm::vec3& velocity, const glm::vec3& gravity)
{

	// Do the collision detection
	collisionPackage->R3Position = this->playerCam->position;
	collisionPackage->R3Velocity = velocity + gravity;

	//  Calculate position and velocity in eSpace
	glm::vec3 eSpacePosition = collisionPackage->R3Position / collisionPackage->eRadius;

	glm::vec3 eSpaceVelocity = collisionPackage->R3Velocity / collisionPackage->eRadius;

	// Iterate until we have final position after a movement event
	collisionRecursiveDepth = 0;

	glm::vec3 finalPosition = collideWithWorld(eSpacePosition, eSpaceVelocity);						// Here

	/*
	// To remove gravity uncomment from here .....
	// Set the new R3 position (convert back from eSpace to R3
	collisionPackage->R3Position =
		finalPosition * collisionPackage->eRadius;
	collisionPackage->R3Velocity = gravity;
	eSpaceVelocity = gravity / collisionPackage->eRadius;
	collisionRecursiveDepth = 0;
	finalPosition = collideWithWorld(finalPosition,
		eSpaceVelocity);
	// ... to here
	*/
	// Convert final result back to R3 (eSpace -> R3)
	finalPosition *= collisionPackage->eRadius;

	// Move the entity (application specific)
	this->playerCam->position = finalPosition;

	//std::cout << "Centre pos FINAL COLLIDE AND SLIDE " << finalPosition[0] << ' ' << finalPosition[1] << ' ' << finalPosition[2] << '\n';

}

// Set to set match the applications scale'
const float unitsPerMeter = 200.0f;



/*


for (const Mesh& bundle : this->tstModel->giveMeshes()) {

		for (int i = 0, s = bundle.indices.size(); i < 2; i += 3) {


			checkTriangle(this->collisionPackage, ((translation * rotation * scale * bundle.vertices[bundle.indices[i]].position) / this->collisionPackage->eRadius), ((translation * rotation * scale * bundle.vertices[bundle.indices[i+1]].position) / this->collisionPackage->eRadius), ((translation * rotation * scale * bundle.vertices[bundle.indices[i+2]].position) / this->collisionPackage->eRadius));

			if (this->collisionPackage->foundCollision) {
				break;
			}

			//std::cout << "AFTER TRIANGLE CHECK COLLIDE POS " << this->collisionPackage->R3Position[0] << ' ' << this->collisionPackage->R3Position[1] << ' ' << this->collisionPackage->R3Position[2] << '\n';

		}

	}


*/

//checkTriangle(this->collisionPackage, ((glm::vec3(this->tstModel->matricesMeshes[i] * glm::vec4(this->tstModel->meshes[i].vertices[this->tstModel->meshes[i].indices[d]].position, 1.0f))) / this->collisionPackage->eRadius), ((glm::vec3(this->tstModel->matricesMeshes[i] * glm::vec4(this->tstModel->meshes[i].vertices[this->tstModel->meshes[i].indices[d + 1]].position, 1.0f))) / this->collisionPackage->eRadius), ((glm::vec3(this->tstModel->matricesMeshes[i] * glm::vec4(this->tstModel->meshes[i].vertices[this->tstModel->meshes[i].indices[d + 2]].position, 1.0f))) / this->collisionPackage->eRadius));

void Player::checkCollision()
{
	//std::cout << "Start collide check...\n";
	
	for (int i = 0; i < this->tstModel->meshes.size(); i++) {

		for (int d = 0; d < this->tstModel->meshes[i].indices.size(); d += 3) {

			//std::cout << this->tstModel->meshes.size() << ' ';

			checkTriangle(this->collisionPackage, ((glm::vec3(this->tstModel->matricesMeshes[i] * glm::vec4(this->tstModel->meshes[i].vertices[this->tstModel->meshes[i].indices[d]].position, 1.0f))) / this->collisionPackage->eRadius), ((glm::vec3(this->tstModel->matricesMeshes[i] * glm::vec4(this->tstModel->meshes[i].vertices[this->tstModel->meshes[i].indices[d + 1]].position, 1.0f))) / this->collisionPackage->eRadius), ((glm::vec3(this->tstModel->matricesMeshes[i] * glm::vec4(this->tstModel->meshes[i].vertices[this->tstModel->meshes[i].indices[d + 2]].position, 1.0f))) / this->collisionPackage->eRadius));

			//std::cout << "AFTER TRIANGLE CHECK COLLIDE POS " << this->collisionPackage->R3Position[0] << ' ' << this->collisionPackage->R3Position[1] << ' ' << this->collisionPackage->R3Position[2] << '\n';

		}

	}

	//std::cout << "\n\n";
	

}


glm::vec3 Player::collideWithWorld(const glm::vec3 ePosition, const glm::vec3 eVelocity) {


	// All hard-coded distances in function are scaled fitting unitsPerMeter
	float unitScale = unitsPerMeter / 100.0f;
	float veryCloseDistance = 0.005f * unitScale;

	// Do we need to check for collision (is far away)
	if (collisionRecursiveDepth > 3) {
		return ePosition;
	}

	//std::cout << "BEFORE Centre pos " << collisionPackage->basePoint[0] << ' ' << collisionPackage->basePoint[1] << ' ' << collisionPackage->basePoint[2] << '\n';

	// Else check
	collisionPackage->velocity = eVelocity;
	collisionPackage->normalizedVelocity = eVelocity;
	glm::normalize(collisionPackage->normalizedVelocity);
	collisionPackage->basePoint = ePosition;
	collisionPackage->foundCollision = false;

	//std::cout << "AFTER Centre pos " << collisionPackage->basePoint[0] << ' ' << collisionPackage->basePoint[1] << ' ' << collisionPackage->basePoint[2] << '\n';

	// Checks for collision for routines
	this->checkCollision();																									// Probs janky collision check routine

	// If no collision move along vector
	if (!collisionPackage->foundCollision) {
		return ePosition + eVelocity;
	}

	//std::cout << "\nCOLLISION RAAAAH " << collisionPackage->basePoint[0] << "\n\n";


	// Else collision

	// Original destination point
	glm::vec3 destinationPoint = ePosition + eVelocity;
	glm::vec3 newBasePoint = ePosition;

	// If not that close to worry ignre
	if (collisionPackage->nearestDistance >= veryCloseDistance) {

		glm::vec3 V = eVelocity;
		V *= ((collisionPackage->nearestDistance - veryCloseDistance) / glm::length(V));				// May be this as collision is clipping
		newBasePoint = collisionPackage->basePoint + V;


		// Adjust poly intersection point so sliding place will be unaffected by the fact that we move slighly less than needed
		glm::normalize(V);
		collisionPackage->intersectionPoint -= veryCloseDistance * V;



	}

	// Determine the sliding plane
	glm::vec3 slidePlaneOrigin = collisionPackage->intersectionPoint;
	glm::vec3 slidePlaneNormal = newBasePoint - collisionPackage->intersectionPoint;

	glm::normalize(slidePlaneNormal);

	Plane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	glm::vec3 newDestinationPoint = destinationPoint - ((float)slidingPlane.signedDistanceTo(destinationPoint) * slidePlaneNormal);

	// Make slide vector will be used for velocity in next iteration
	glm::vec3 newVelocityVector = newDestinationPoint - collisionPackage->intersectionPoint;


	// Recurse time Yippe!

	// Dont recurse if too short of jumps
	if (glm::length(newVelocityVector) < veryCloseDistance) {

		return newBasePoint;

	}

	collisionRecursiveDepth++;

	return collideWithWorld(newBasePoint, newVelocityVector);



}