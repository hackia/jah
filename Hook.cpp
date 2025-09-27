#include "Hook.hpp"
#include <cstring>
#include <iostream>
#include <ostream>
#include <filesystem>

using namespace Jah;
using namespace std;
namespace fs = std::filesystem;

Hook::Hook(const Language language) {
  switch (language) {
  case C:
    this->to_run.emplace_back("Run CMake", "cmake .", "CMake success",
                              "CMake failure");
    this->to_run.emplace_back("Run make", "make", "Make success",
                              "Make failure");
    this->to_run.emplace_back("Run tests", "make test", "Tests success",
                              "Tests failure");
    break;
  case Rust:
    this->to_run.emplace_back("Run cargo check", "cargo check", "Check success",
                              "Check failure");
    this->to_run.emplace_back("Run make", "make", "Make success",
                              "Make failure");
    this->to_run.emplace_back("Run tests", "make test", "Tests success",
                              "Tests failure");
    break;
  case NodeJs:
    this->to_run.emplace_back("Run npm doctor", "npm doctor", "Doctor success",
                              "Doctor detected failure");
    this->to_run.emplace_back("Check dependencies", "npm audit",
                              "No vulnerabilities", "Found vulnerabilities");
    this->to_run.emplace_back("Run tests", "npm run test", "Tests success",
                              "Tests failure");
    break;
  case PYTHON:
    this->to_run.emplace_back("Run pylint", "pylint *.py", "Linting success",
                              "Linting failure");
    this->to_run.emplace_back("Run mypy", "mypy .", "Type check success",
                              "Type check failure");
    this->to_run.emplace_back("Run pytest", "pytest", "Tests success",
                              "Tests failure");
    break;
  case JAVA:
    this->to_run.emplace_back("Run Maven compile", "mvn compile",
                              "Compilation success", "Compilation failure");
    this->to_run.emplace_back("Run Maven verify", "mvn verify",
                              "Verification success", "Verification failure");
    this->to_run.emplace_back("Run tests", "mvn test", "Tests success",
                              "Tests failure");
    break;
  }
}

int Hook::run() {
  vector<Language> l;
  for (const auto support = {"C", "Rust", "NodeJs", "PYTHON", "JAVA", "C"};
       auto &s : support) {
    if (strcmp(s, "C") == 0 && fs::exists("CMakeLists.txt")) {
      l.push_back(C);
    }
    if (strcmp(s, "Rust") == 0 && fs::exists("Cargo.toml")) {
      l.push_back(Rust);
    }
    if (strcmp(s, "NodeJs") == 0 && fs::exists("package.json")) {
      l.push_back(NodeJs);
    }
    if (strcmp(s, "PYTHON") == 0 && fs::exists(".py")) {
      l.push_back(PYTHON);
    }
    if (strcmp(s, "JAVA") == 0 && fs::exists("pom.xml")) {
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

Hook *Hook::preCommit() {
  for (auto &r : this->to_run) {
    if (system(std::get<1>(r).c_str()) == 0) {
      cout << std::get<2>(r) << endl;
      this->ok++;
    } else {
      cerr << std::get<3>(r) << endl;
      this->ko++;
    }
  }
  return this;
}

int Hook::finally() const { return this->ko > 0 ? 0 : 1; }
