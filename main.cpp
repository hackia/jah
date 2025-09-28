#include "Controller.hpp"
#include "View.hpp"
using namespace Jah;
using namespace std;
namespace fs = std::filesystem;
int main(const int argc, const char **argv) {
  if (fs::exists(".jah/jah.db")) {
    const Config config;
    constexpr Observer observer = DatabaseAdmin;
    const Controller controller(config, observer);
    return controller.run(argc, argv);
  }
  Config config;
  return config.initialize();
}
