#include "Hook.hpp"

#include <iostream>
#include <ostream>

using namespace Jah;
using namespace std;
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
