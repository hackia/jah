#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace Jah {

enum Language {
  C,
  Rust,
  NodeJs,
  PYTHON,
  JAVA,
};
class Hook {
public:
  /**
   * @brief Constructs a Hook object using the specified programming language
   * and Atom instance.
   *
   * This constructor initializes the Hook object with a given language type
   * and an Atom instance, which represents specific content or rules that
   * the Hook will operate with.
   *
   * @param language The programming language that the Hook will be associated
   * with. It is represented by an enumerated value from the Language
   *                 enumeration.
   *
   * @return An initialized Hook object configured with the given language and
   * Atom details.
   */
  explicit Hook(Language language);
  static int run();
  Hook *preCommit();
  [[nodiscard]] int finally() const;

private:
  unsigned int ko = 0;
  unsigned int ok = 0;
  std::vector<std::tuple<std::string, std::string, std::string, std::string>>
      to_run;
};
} // namespace Jah
