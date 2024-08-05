#include "cleaning.hpp"
#include <stdexcept>
#include <vector>
#include <GL/glew.h>

void normalize_coords(std::vector<GLfloat>& data, GLfloat lat, GLfloat lon){
  if (data.size() % 2 != 0) {
    throw std::runtime_error("The coordinates vector size is not a multiple of 2");
  }

  for(decltype(data.size()) i = 0; i < data.size(); i+=2){
    if (i + 1 >= data.size()) {
      throw std::runtime_error("Index out of bounds while accessing coordinates");
    }
    data[i] -= lat;
    data[i + 1] -= lon;
  }
}
