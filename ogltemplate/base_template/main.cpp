
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "common/shader.hpp"

using namespace glm;


void draw_triangle(GLuint* VertexArrayID, GLuint* vertexbuffer){
  if(VertexArrayID == nullptr){
      throw std::invalid_argument("vertexarrayid is a nullptr");
  }
  glGenVertexArrays(1, VertexArrayID);
  glBindVertexArray(*VertexArrayID);

  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
  };

  glGenBuffers(1, vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

int main(){
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

  glClearColor(0.0f, 0.4f, 0.4f, 0.0f);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  
  GLuint vertexbuffer;
  GLuint VertexArrayID;
  draw_triangle(&VertexArrayID, &vertexbuffer);

  GLuint programID = LoadShaders("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

  do{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glUseProgram(programID);
    glfwSwapBuffers(window);
    glfwPollEvents();
  } 
  while( 
    glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS 
    && glfwWindowShouldClose(window) == 0 
  );
  return 0;
}
