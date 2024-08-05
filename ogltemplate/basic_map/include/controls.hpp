#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 getRotationMatrix();

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif
