#ifndef SYS_H
#define SYS_H
#include <string>

void open_preview(std::string filepath) {
#ifdef __APPLE__
  system(std::string("open " + filepath).c_str());
#else
  std::cerr << "Preview only supported on Mac";
#endif
}

#endif
