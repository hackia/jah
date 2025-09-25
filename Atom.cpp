#include "Atom.hpp"

using namespace Jah;

Atom::Atom(string type, string summary, string body, string footer)
    : type(std::move(type)), summary(std::move(summary)),
      footer(std::move(footer)), body(std::move(body)) {}

string Atom::getType() const { return type; }
string Atom::getSummary() const { return summary; }
string Atom::getBody() const { return body; }
string Atom::getFooter() const { return footer; }

bool Atom::containsIssue(const unsigned int id) const {
  return footer.find("issue") != string::npos &&
         footer.find(to_string(id)) != string::npos;
}

int Atom::accepted() { return ATOM_ACCEPTED; }
int Atom::refused() { return ATOM_REFUSED; }
