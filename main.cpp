#include "Atom.hpp"
#include "Hook.hpp"

#include <filesystem>
#include <iostream>

#include "Ji.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <vector>

using namespace Jah;
using std::string;
using namespace std;
namespace fs = std::filesystem;

static int runHook() {
  const auto support = {"C", "Rust", "NodeJs", "PYTHON", "JAVA", "C"};
  vector<Language> l;
  for (auto &s : support) {
    if (strcmp(s, "C") == 0 && exists("CMakeLists.txt")) {
      l.push_back(C);
    }
    if (strcmp(s, "Rust") == 0 && exists("Cargo.toml")) {
      l.push_back(Rust);
    }
    if (strcmp(s, "NodeJs") == 0 && exists("package.json")) {
      l.push_back(NodeJs);
    }
    if (strcmp(s, "PYTHON") == 0 && exists(".py")) {
      l.push_back(PYTHON);
    }
    if (strcmp(s, "JAVA") == 0 && exists("pom.xml")) {
      l.push_back(JAVA);
    }
  }
  for (const auto &s : l) {
    if (Hook h(s); h.preCommit()->finally() == false) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

static std::string ask(const std::string &prompt,
                       const std::vector<std::string> &samples) {
  std::string input;

  do {
    std::cout << prompt << ": ";
    if (!samples.empty()) {
      std::cout << "[ ";
      for (size_t i = 0; i < samples.size(); ++i) {
        std::cout << samples[i] << (i == samples.size() - 1 ? "" : ", ");
      }
      std::cout << " ]";
    }
    std::cout << std::endl << "> ";

    std::getline(std::cin, input);

    if (input.empty()) {
      std::cout << "Input cannot be empty. Please try again." << std::endl;
    }

  } while (input.empty());

  return input;
}

static int initialize(const int argc, const char **argv) {
  cout << "Initializing Jah object database..." << endl;
  // Store the results in std::string variables.
  const std::string hostname =
      ask("PG_HOST", {"localhost", "0.0.0.0", "127.0.0.1"});
  const std::string port = ask("PG_PORT", {"5432", "5433"});
  const std::string dbname = ask("PG_DBNAME", {"jah", "db_name"});
  const std::string user = ask("PG_USER", {"postgres"});
  const std::string password = ask("PG_PASSWORD", {"postgres"});
  const std::string username = ask("USERNAME", {"hackia"});
  const std::string email = ask("EMAIL", {"dev@hackia.org"});

  if (const bool confirm = ask("confirm creation?", {"yes", "no"}) == "yes";
      !confirm) {
    return initialize(argc, argv);
  }
  return Ji::init(hostname, port, dbname, user, password, username, email);
}

static int commit() {
  if (runHook() == EXIT_SUCCESS) {

    cout << "Committing..." << endl;
    const string type = ask("Commit type", {"feat", "chore"});
    const string summary = ask("Commit summary", {"a small description"});
    const string body = ask("Commit body", {"a complete description"});
    const string footer =
        ask("Commit footer", {"a complete description for footer"});
    const string resolve = ask("Commit linked issue", {"#0001", "#00011"});

    std::cout << "\n--- Commit Summary ---" << std::endl;
    std::cout << "type : " << type << endl;
    std::cout << "summary : " << summary << endl;
    std::cout << "body : " << body << endl;
    std::cout << "footer : " << footer << endl;
    std::cout << "resolve : " << resolve << endl;

    if (ask("confirm", {"yes", "no"}) == "yes") {
      const Atom atom(type, summary, body, footer, resolve);
      return atom.save();
    }
    return commit();
  }
  cerr << "hook has fail" << endl;
  return EXIT_FAILURE;
}

static int help(const char **argv) {
  cout << "Usage: " << argv[0] << " [command]" << endl;
  cout << "Commands:" << endl;
  cout << "  log      Show commit log" << endl;
  cout << "  commit   Commit changes" << endl;
  cout << "  health   Check health status" << endl;
  cout << "  init     Initialize repository" << endl;
  cout << "  help     Show help" << endl;
  cout << endl;
  return EXIT_SUCCESS;
}
int parse(const int argc, const char **argv) {

  if (argc == 2) {
    if (strcmp(argv[1], "log") == 0) {
      return Atom::log(50);
    }
    if (strcmp(argv[1], "commit") == 0) {
      return commit();
    }
    if (strcmp(argv[1], "health") == 0) {
      return runHook();
    }
    if (strcmp(argv[1], "init") == 0) {
      return initialize(argc, argv);
    }
    if (strcmp(argv[1], "help") == 0) {
      return help(argv);
    }
  }
  std::cerr << "Error: missing command" << std::endl;
  return EXIT_FAILURE;
}

int main(const int argc, const char **argv) { return parse(argc, argv); }
