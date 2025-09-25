#include "Singularity.hpp"
#include "Hook.hpp"
#include "Ji.hpp"
#include <stdexcept>
using namespace Jah;

Singularity::Singularity(const unsigned int id, Atom *atom,
                         const Language language, Hook *hook) {
  if (atom == nullptr) {
    throw std::invalid_argument("Atom pointer cannot be null");
  }
  if (hook == nullptr) {
    throw std::invalid_argument("Hook pointer cannot be null");
  }
  this->issue = id;
  this->lang = language;
  this->h = hook;
  this->a = atom;
  this->accepted = false;
}

bool Singularity::check() {
  if (this->h == nullptr) {
    throw std::runtime_error("Hook pointer is null");
  }
  if (this->a == nullptr) {
    throw std::runtime_error("Atom pointer is null");
  }
  if (!this->a->containsIssue(issue)) {
    throw std::runtime_error("No issue detected in Atom");
  }
  switch (this->lang) {
  case C:
    if (filesystem::exists("CMakeLists.txt") == false) {
      throw std::runtime_error("CMakeLists.txt not found");
    }
    break;
  case Rust:
    if (filesystem::exists("Cargo.toml") == false) {
      throw std::runtime_error("Cargo.toml not found");
    }
    break;
  case NodeJs:
    if (filesystem::exists("package.json") == false) {
      throw std::runtime_error("package.json not found");
    }
    break;
  case PYTHON:
    if (filesystem::exists("setup.py") == false) {
      throw std::runtime_error("setup.py not found");
    }
    break;
  case JAVA:
    if (filesystem::exists("pom.xml") == false) {
      throw std::runtime_error("pom.xml not found");
    }
    break;
  default:
    throw std::runtime_error("Unknown language");
  }
  if (this->h->preCommit()) {
    this->accepted = true;
  }
  return this->accepted;
}

bool Singularity::isAccepted() const { return this->accepted; }