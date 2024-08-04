#include <glm/gtc/matrix_transform.hpp>


void model(float height, float width){
  glm::mat4 Projection = glm::perspective(
    glm::radians(45.0f),
    (static_cast<float>(width) / static_cast<float>(height)),
    0.1f,
    100.0f
  );
  glm::mat4 View = glm::lookAt(
    glm::vec3(4,3,3), // camera at (4,3,3) in world space
    glm::vec3(0,0,0), // looks at origin
    glm::vec3(0,1,0) // head is up
  );
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 mvp = Projection * View * Model;
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
}
