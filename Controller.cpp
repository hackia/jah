#include "Controller.hpp"
#include "Atom.hpp"
#include "Hook.hpp"
#include "Ji.hpp"

#include <cstring>
#include <iostream>
using namespace std;
using namespace Jah;


enum class Category {
  Core,
  Events,
  Communication,
  Development,
  Maintenance,
  Process,
  Creative,
  Chaos
};

struct CosmicTypes {
  std::string category;
  std::vector<std::string> types;
};

static std::vector<CosmicTypes> getCosmicTypeMap() {
  return {
      {"Core", {"Star", "Comet", "Nebula", "Quasar", "Pulsar"}},
      {"Events", {"Black Hole", "Wormhole", "Big Bang", "Launch"}},
      {"Communication",
       {"Lightspeed", "Mission Control", "Spacewalk", "Moon Landing",
        "First Contact", "Interstellar Communication"}},
      {"Development",
       {"Solar Flare", "Dwarf Planet", "Terraform", "Red Giant", "Neutron Star",
        "Brown Dwarf"}},
      {"Maintenance",
       {"Asteroid Belt", "White Dwarf", "Time Dilation", "Gravity",
        "Dark Matter", "Hawking Radiation"}},
      {"Process",
       {"Space Station", "Rocket Launch", "Space Probe", "Space Elevator"}},
      {"Creative", {"Spark", "Palette", "Poem", "Stage", "Blueprint"}},
      {"Chaos",
       {"Big Crunch", "Supernova", "Black Hole", "Wormhole", "Dark Matter"}}};
}

static void typeHelp() {
  const auto types = getCosmicTypeMap();
  cout << "Types" << endl;
  for (size_t i = 0; i < types.size(); ++i) {
    const auto &category = types[i];
    cout << (i == types.size() - 1 ? "└── " : "├── ") << category.category
         << endl;
    for (size_t j = 0; j < category.types.size(); ++j) {
      cout << "│     " << (j == category.types.size() - 1 ? "└── " : "├── ")
           << category.types[j] << endl;
    }
  }
}

Controller::Controller(const Config &config, const Observer &observer)
    : config(config), model(config), view(observer), observer(observer) {}

string Controller::ask(const string &question, const vector<string> &samples) {
  std::string input;

  do {
    std::cout << question << ": ";
    if (!samples.empty()) {
      std::cout << "[ ";
      for (size_t i = 0; i < samples.size(); ++i) {
        std::cout << samples[i] << (i == samples.size() - 1 ? "" : ", ");
      }
      std::cout << " ]";
    }
    std::cout << std::endl << "> ";

    std::string line;
    while (std::getline(std::cin, line)) {
      if (line.empty()) {
        break;
      }

      if (!input.empty()) {
        input += "\n";
      }
      input += line;
    }
    if (input.empty()) {
      std::cout << "Input cannot be empty. Please try again." << std::endl;
    }

  } while (input.empty());

  return input;
}

int Controller::initialize() {
  cout << "Initializing Jah object database..." << endl;
  const std::string hostname =
      ask("PG_HOST", {"localhost", "0.0.0.0", "127.0.0.1"});
  const std::string port = ask("PG_PORT", {"5432", "5433"});
  const std::string dbname = ask("PG_DBNAME", {"jah", "db_name"});
  const std::string user = ask("PG_USER", {"postgres"});
  const std::string password = ask("PG_PASSWORD", {"postgres"});
  const std::string username = ask("USERNAME", {"hackia"});
  const std::string email = ask("EMAIL", {"dev@hackia.org"});
  return Ji::init(hostname, port, dbname, user, password, username, email);
}
int Controller::commit(const Config &config) {
  cout << "Committing controller..." << endl;
  if (const std::string type = ask("type", {"type help"}); type == "help") {
    typeHelp();
    return commit(config);
  }
  const std::string type = ask("type", {"type help for show all"});
  const std::string summary = ask("summary", {"a small description"});
  const std::string body = ask("body", {"a complete description"});
  const std::string note = ask("note", {"note for the teams"});
  const std::string footer = ask("footer", {"footer of the note"});
  const std::string id = ask("issue", {"#0001", "#0011"});
  const Atom atom(type, summary, body, note, footer, id);
  return atom.save(config);
}
int Controller::run(const int argc, const char **argv) const {
  if (argc == 2) {
    if (strcmp(argv[1], "log") == 0) {
      return Atom::log(50, config);
    }
    if (strcmp(argv[1], "ls") == 0) {
      return view.ls();
    }
    if (strcmp(argv[1], "commit") == 0) {
      return commit(config);
    }
    if (strcmp(argv[1], "health") == 0) {
      return Hook::run();
    }
    if (strcmp(argv[1], "init") == 0) {
      return initialize();
    }
    if (strcmp(argv[1], "help") == 0) {
      return help(argv);
    }
  }
  std::cerr << "Error: missing command" << std::endl;
  return EXIT_FAILURE;
}

int Controller::help(const char **argv) {
  cout << "Usage: " << argv[0] << " [command]" << endl;
  cout << "Commands:" << endl;
  cout << "  log      Show commit log" << endl;
  cout << "  ls       Show commit log" << endl;
  cout << "  reset    Reset database" << endl;
  cout << "  commit   Commit changes" << endl;
  cout << "  health   Check code health" << endl;
  cout << "  init     Initialize repository" << endl;
  cout << "  help     Show help" << endl;
  cout << endl;
  return EXIT_SUCCESS;
}
