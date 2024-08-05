#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "file_handling.hpp"
#include "buffering.hpp"
#include "cleaning.hpp"
#include "controls.hpp"

using namespace glm;


const GLfloat BASE_LAT = 59.2373418; 
const GLfloat BASE_LON = 17.8365887;


int main(int argc, char** argv){
  if(argv == nullptr){
    std::cerr << "Missing input file flags"<< std::endl;
  }

  glewExperimental = true;
  if (!glfwInit()){
    fprintf(stderr, "failed initialize glfw\n");
    return -1;
  }
  glfwWindowHint(GLFW_SAMPLES, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  GLFWwindow* window; 
  window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
  if( window == NULL ){
    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window); 
  glewExperimental=true; 
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwPollEvents();
  glfwSetCursorPos(window, 1024/2, 768/2);

  std::string filename = get_filename(argc, argv);
  std::ifstream file(filename);
  std::vector<GLfloat> coordinates;
  if(file){
    GLfloat iVal; 
    while(file >> iVal){
      coordinates.push_back(iVal);
    }
  } else {
    std::cerr << "Could not open file: " << filename << std::endl;
    return -1;
  }

  normalize_coords(coordinates, BASE_LAT, BASE_LON);

  GLuint vertexbuffer, VertexArrayID;
  draw_triangle(&VertexArrayID, &vertexbuffer, coordinates);

  GLuint programID = LoadShaders("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
  // glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  GLuint UniformColorID = glGetUniformLocation(programID, "uniformcolor"); 

  float i = 0.0f;

  do {
    glClear( GL_COLOR_BUFFER_BIT );
    glUseProgram(programID);
    
    // Compute the MVP matrix from keyboard and mouse input
    computeMatricesFromInputs(window);
    glm::mat4 ProjectionMatrix = getProjectionMatrix();
    glm::mat4 ViewMatrix = getViewMatrix();
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 RotationMatrix = getRotationMatrix();
    glm::mat4 MVP = RotationMatrix * ProjectionMatrix * ViewMatrix * ModelMatrix;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
      0,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
    );

    glUniform3f(UniformColorID, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_POINTS, 0, coordinates.size()/2);

    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
    glfwPollEvents();

    if(i > 1){
      i = 0.0f;
    } else {
      i += 0.01f;
    }

  } while( 
  glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS 
  && glfwWindowShouldClose(window) == 0 
);

  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  return 0;
}
