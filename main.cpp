#include <iostream>

#include "Ji.hpp"
#include "cxxopts.hpp"

using namespace Dualys;

int main(int argc, char** argv) {
  cxxopts::Options options("Jah", "A team");

  options.add_options()("i,init", "init the repository")("h,help",
                                                         "Print usage");

  Jah* app = new Jah();
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return EXIT_SUCCESS;
  }
  if (result.count("init")) {
    return app->init();
  }
  return EXIT_FAILURE;
}
