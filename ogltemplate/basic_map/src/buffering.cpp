#include "buffering.hpp"
#include <stdexcept>

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
    0.0f, 1.0f, 0.0f,

    0.5f, -1.0f, 0.0f,
    1.0f, -1.0f, 1.0f,
    0.5f, 1.0f, 0.0f,
  };

  glGenBuffers(1, vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer); // generate one buffer and store its name in vertexbuffer var
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}
