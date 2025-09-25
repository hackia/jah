#include "Queue.hpp"
#include <algorithm>
#include <vector>

using namespace Jah;
Queue::Queue() : all(std::vector<Atom>()) {}

void Queue::add(const Atom &atom) { this->all.push_back(atom); }

void Queue::del(const Atom &atom) {
  const auto it =
      std::find_if(this->all.begin(), this->all.end(), [&atom](const Atom &a) {
        return a.getType() == atom.getType() &&
               a.getSummary() == atom.getSummary() &&
               a.getBody() == atom.getBody() &&
               a.getFooter() == atom.getFooter();
      });
  if (it != this->all.end()) {
    this->all.erase(it);
  }
}
Atom Queue::first() { return this->all.front(); }
Atom Queue::last() { return this->all.back(); }
