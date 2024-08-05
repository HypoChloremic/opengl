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

using namespace glm;

struct Coord {
  GLfloat lat;
  GLfloat lon;
};

int main(int argc, char** argv){
  if(argv == nullptr){
    std::cerr << "Missing input file flags"<< std::endl;
  }
  std::string filename = get_filename(argc, argv);
  std::ifstream file(filename);
  std::vector<GLfloat> coordinates;
  if(file){
    GLfloat iVal; 
    while(!file.eof()){
      file >> iVal;
      coordinates.push_back(iVal);
    }
  } else {
    std::cerr << "Could not open file: " << filename << std::endl;
    return -1;
  }
  for (const auto& coordinate : coordinates) {
    std::cout << coordinate << std::endl; 
  }
  return 0;
}

int main1(){
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
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  GLuint vertexbuffer;
  GLuint VertexArrayID;
  draw_triangle(&VertexArrayID, &vertexbuffer);

  GLuint programID = LoadShaders("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 View = glm::lookAt(
    glm::vec3(10,3,3),
    glm::vec3(0,0,0),
    glm::vec3(0,2,0)
  );

  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 mvp = Projection * View * Model;
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  GLuint UniformColorID = glGetUniformLocation(programID, "uniformcolor"); 

  float i = 0.0f;

  do {
    glClear( GL_COLOR_BUFFER_BIT );
    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
    );

    glUniform3f(UniformColorID, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    glUniform3f(UniformColorID, i, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 3, 3);

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
