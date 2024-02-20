#include "playerCamera.h"
#include "entity.h"

#include <chrono>
#include <thread>

// Use a random device to seed the random number generator
std::random_device timeStamp;
// Use the Mersenne Twister engine
std::mt19937 seed(timeStamp());

// Define a uniform distribution between 1 and 100
std::uniform_real_distribution<float> generateRandomCoordPos(-15.5f, 15.5f);
std::uniform_real_distribution<float> generateRandomSpeed(0.5f, 5.5f);

// Set dimensions of window
unsigned short int width = 2000, height = 2000;

void populateMap(Entity**& entityBuffer)
{

	// Use a random device to seed the random number generator
	std::random_device timeStamp;
	// Use the Mersenne Twister engine
	std::mt19937 seed(timeStamp());

	// Define a uniform distribution between 1 and 100
	std::uniform_real_distribution<float> generateRandomCoordPos(-15.5f, 15.5f);
	std::uniform_real_distribution<float> generateRandomSpeed(0.5f, 5.5f);

	for (int i = 0; i < 10; i++) {

		entityBuffer[i] = new Entity("models/Silly_Enemy/scene.gltf", 0, glm::vec3(generateRandomCoordPos(seed), 0.0f, generateRandomCoordPos(seed)), generateRandomSpeed(seed));

	}

}

int main()
{
	// Firing up GLFW which will be renderer
	glfwInit();

	// Tell it our version of OpenGL it will be dabbling in (OpenGl 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// We are taking in modern functions, cutting out old alias functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Our window! Yippe!
	GLFWwindow* local_Window = glfwCreateWindow(width, height, "Penus", NULL, NULL);

	// Telling computer to watch window and pass user to it
	glfwMakeContextCurrent(local_Window);
	
	glfwSetWindowPos(local_Window, 1200, 50);

	// Glad will help setup GL
	gladLoadGL();

	// Set current eyes of window
	glViewport(0,0, width,height);


	// Initialize the base shader
	Shader shaderProg("default.vert", "default.frag");


	// Base color of light source
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the position of the light source and its models filling. move the light 
	glm::vec3 lightPos = glm::vec3(0.2f, 0.8f, 0.4f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	
	// Turn on base shader and feed in light model and its data as well as shaded models
	shaderProg.Activate();
	glUniform4f(glGetUniformLocation(shaderProg.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProg.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	// Enable GL to mesh occlude
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	const int targetFR = 144;

	const std::chrono::milliseconds frameDuration(1000 / targetFR);

	std::chrono::steady_clock::time_point frameStart, frameEnd;
	std::chrono::milliseconds frameTime;
	
	

	Model han("models/gunkyUpdate/scene.gltf", 1);
	
	Entity** enemyBatch = new Entity*[10];

	populateMap(enemyBatch);

	//Entity enemy("models/Silly_Enemy/scene.gltf", 0, glm::vec3(0.0f, 0.0f, 0.0f));

	Player player(width, height, glm::vec3(0.0f, 1.0f, 2.0f), han);																											// make constructor passing in playerCam as its 
	int fill = 10;

	han.genWalls(1);


	bool debug = 0;


	// Main loop
	while (!glfwWindowShouldClose(local_Window)) {
		frameStart = std::chrono::high_resolution_clock::now();

		// Background color
		glClearColor(0.0f, 0.0f, 0.0f, 1);
		// Clear back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.playerCam->updateMatrix(95.0f, 0.1f, 100.0f);									
		
		glUniform3f(glGetUniformLocation(shaderProg.ID, "lightPos"), player.playerCam->position.x, player.playerCam->position.y, player.playerCam->position.z);
		glUniform3f(glGetUniformLocation(shaderProg.ID, "playerOrient"), player.playerCam->orientation.x, player.playerCam->orientation.y, player.playerCam->orientation.z);

		han.Draw(shaderProg, *player.playerCam);

		if (player.bulletBin) {
			player.bulletBin->shift(shaderProg, *player.playerCam);
			if (player.bulletBin->checkCollisions(enemyBatch) == true) {

				delete player.bulletBin;
				player.bulletBin = nullptr;

			}
		}

		for (int i = 0; i < 10; i++) {

			if (!enemyBatch[i]) {

				enemyBatch[i] = new Entity("models/Silly_Enemy/scene.gltf", 0, glm::vec3(generateRandomCoordPos(seed), 0.0f, generateRandomCoordPos(seed)), generateRandomSpeed(seed));

			}

			enemyBatch[i]->react(shaderProg, *player.playerCam);

		}



		/*
		if (player.bulletBin) {

			player.bulletBin->EB1.unBind();
			player.bulletBin->VB1.unBind();
			player.bulletBin->VA1.unBind();

			player.bulletBin->EB1.Delete();
			player.bulletBin->VB1.Delete();
			player.bulletBin->VA1.Delete();

			delete player.bulletBin;
			player.bulletBin = nullptr;
		}
		*/
		

		// Goto new cleaned buffer
		glfwSwapBuffers(local_Window);

		// Watch events
		glfwPollEvents();

		frameEnd = std::chrono::high_resolution_clock::now();
		frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

		if (frameTime < frameDuration) {

			player.inputs(local_Window, shaderProg);
			auto sleepTime = frameDuration - frameTime;
			std::this_thread::sleep_for(sleepTime);
		}

	}



	// Cleanup window
	shaderProg.Delete();

	glfwDestroyWindow(local_Window);
	glfwTerminate();

	return 1;

}


