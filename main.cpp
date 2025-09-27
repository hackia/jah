#include "Atom.hpp"
#include "Controller.hpp"
#include "View.hpp"

using namespace Jah;

int main(const int argc, const char **argv) {
  const Config configuration("jah", "jah", "localhost", "5432", "jah");
  constexpr Observer observer = FullStackDev;
  const Controller controller(configuration, observer);
  return controller.run(argc, argv);
}
