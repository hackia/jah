#include "Singularity.hpp"
#include "Hook.hpp"
using namespace Jah;

Singularity::Singularity(const unsigned int id, Atom *atom,
                         const Language language, Hook *hook) {
  this->issue = id;
  this->lang = language;
  this->h = hook;
  this->a = atom;
  this->isAccepted = false;
}
bool Singularity::check() {
  if (this->h->preCommit()) {
    this->isAccepted = true;
  }
  return this->isAccepted;
}
