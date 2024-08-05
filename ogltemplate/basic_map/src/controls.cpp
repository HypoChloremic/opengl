// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 RotationMatrix = glm::mat4(1.0f);

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 getRotationMatrix(){
	return RotationMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 0.1f; // 3 units / second
float mouseSpeed = 1e-5f;

float oLeft = -0.1f;
float oRight = 0.1f;
float oBottom = -0.1f;
float oTop = 0.1f;
float oNear = 0.01f;
float oFar = 100.0f;
float zoom = 2.0f;

float rotation = 0.0f;

// Global variable to store the scroll offset
static double scrollOffset = 0.0;

// Scroll callback function
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	scrollOffset += yoffset;
}

void computeMatricesFromInputs(GLFWwindow* window){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	// glfwGetMousePos(&xpos, &ypos);
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
		oTop -= oTop * deltaTime * speed;
		oBottom -= oBottom * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
		oTop += oTop * deltaTime * speed;
		oBottom += oBottom * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
		oRight +=  oRight * deltaTime * speed;
		oLeft -=  oLeft * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
		oLeft +=  oLeft * deltaTime * speed;
		oRight -=  oRight * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_O ) == GLFW_PRESS){
		oNear += oNear * deltaTime * speed;
		oFar  -= oFar * deltaTime * speed;
		std::cout << "oNear " << oNear << "oFar " << oFar << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_I ) == GLFW_PRESS){
		oNear -= oNear * deltaTime * speed;
		oFar  += oFar * deltaTime * speed;
		std::cout << "oNear " << oNear << "oFar " << oFar << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Z ) == GLFW_PRESS){
		zoom += (zoom * deltaTime * 2.0f) / 14.0f; // 114% zoom in  
		std::cout << "zoom " << zoom << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_X ) == GLFW_PRESS){
		zoom -= (zoom * deltaTime * 2.0f) / 14.0f; // 114% zoom in  
		std::cout << "zoom " << zoom << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_R ) == GLFW_PRESS){
		RotationMatrix = glm::rotate(RotationMatrix, glm::radians(rotation+=0.01f), glm::vec3(0.0f, 0.0f, 1.0f));
		if(rotation > 360.0f) {
			rotation = 0.0f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		RotationMatrix = glm::rotate(RotationMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glfwSetScrollCallback(window, scroll_callback);

	float FoV = initialFoV - 5 * scrollOffset;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	// ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Orthographic projection parameters

	// Projection matrix: Orthographic projection
	ProjectionMatrix = glm::ortho(oLeft/zoom, oRight/zoom, oBottom/zoom, oTop/zoom, oNear/zoom, oFar/zoom);

	// Camera matrix
	ViewMatrix       = glm::lookAt(
		position,           // Camera is here
		position+direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);


	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
