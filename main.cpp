#include "Atom.hpp"

#include <filesystem>
#include <iostream>

#include "Ji.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>

using namespace Jah;
using std::string;

namespace fs = std::filesystem;
string prompt(const string &message,
              const vector<string> &values = vector<string>()) {
  std::string ligne;
  std::string x;
  bool last_line_empty = false;
  if (!values.empty()) {
    string joined;
    std::cout << "valid values are [ ";
    for (size_t i = 0; i < values.size(); ++i) {
      if (i > 0)
        joined += ", ";
      joined += values[i];
    }
    cout << joined << " ]" << endl;
  }

  cout << message;
  while (std::getline(std::cin, ligne)) {
    if (ligne.empty()) {
      if (last_line_empty) {
        break;
      }
      last_line_empty = true;
    } else {
      last_line_empty = false;
    }
    x += ligne + "\n";
  }
  if (x.length() >= 2) {
    x.resize(x.length() - 2);
  }
  if (const bool founded = find(values.begin(), values.end(), x) != values.end();
      !values.empty() && !x.empty() && founded) {
    throw std::runtime_error("Invalid value");
  }
  return x;
}

int main(const int argc, const char **argv) {
  if (argc < 2) {
    std::cerr << "Error: missing command" << std::endl;
    return EXIT_FAILURE;
  }

  if (argc == 2 && strcmp(argv[1], "init") == 0) {
    return Ji::init();
  }
  if (argc == 2 && strcmp(argv[1], "commit") == 0) {

    const Language l = fs::exists("CMakeLists.txt") ? C
                       : fs::exists("Cargo.toml")   ? Rust
                                                    : NodeJs;

    if (Hook hook(l); hook.preCommit()->finally()) {
      string type, summary, body, footer, issues;
      do {
        try {
          type = prompt("Type: ",
                        std::vector<std::string>{
                            "nova", "black hole", "stardust", "constellation",
                            "starmap", "gravity", "bigbang"});
        } catch (std::runtime_error &e) {
          cerr << e.what() << endl;
          type.clear();
          continue;
        }
        summary = prompt("Summary: ");
        body = prompt("Body: ");
        footer = prompt("Footer: ");
        issues = prompt("Issue: ");
      } while (type.empty() || summary.empty() || body.empty() ||
               footer.empty() || issues.empty());
      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
  }
  return EXIT_FAILURE;
}
