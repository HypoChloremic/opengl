#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "common/shader.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


void draw_triangle(GLuint* VertexArrayID, GLuint* vertexbuffer){
  if(VertexArrayID == nullptr){
    throw std::invalid_argument("vertexarrayid is a nullptr");
  } else if (vertexbuffer == nullptr) {
    throw std::invalid_argument("vertexbuffer is a nullptr");
  }

  glGenVertexArrays(1, VertexArrayID);
  glBindVertexArray(*VertexArrayID);

  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 1.0f,
  };

  glGenBuffers(1, vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer); // generate one buffer and store its name in vertexbuffer var
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

    glUniform3f(UniformColorID, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_POINTS, 0, 2);

    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
    glfwPollEvents();

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
