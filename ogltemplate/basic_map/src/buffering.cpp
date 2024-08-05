#include "buffering.hpp"
#include <stdexcept>
#include <vector>
#include <GL/glew.h>

void draw_triangle(GLuint* VertexArrayID, GLuint* vertexbuffer, std::vector<GLfloat>& vertexdata){
  if(VertexArrayID == nullptr || vertexbuffer == nullptr){
    throw std::invalid_argument("vertexarrayid is a nullptr");
  }

  glGenVertexArrays(1, VertexArrayID);
  glBindVertexArray(*VertexArrayID);

  glGenBuffers(1, vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer); // generate one buffer and store its name in vertexbuffer var
  glBufferData(GL_ARRAY_BUFFER, vertexdata.size() * sizeof(GLfloat), vertexdata.data(), GL_STATIC_DRAW);
}
