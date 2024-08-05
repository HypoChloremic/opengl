#include "file_handling.hpp"
#include <iostream>


std::string get_filename(int argc, char** argv){
  if(argv == nullptr){
    std::cerr << "Missing input file flags"<< std::endl;
  }

  if (argc <= 1) {
    throw std::runtime_error("insufficient flags provided");
  }

  std::string filename = argv[1];

  return filename;
}
