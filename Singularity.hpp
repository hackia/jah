#pragma once

#include "Atom.hpp"

#include <cxxopts.hpp>
namespace Jah {

class Singularity {
public:
  /**
   * @brief Constructs a new Singularity object.
   *
   * @param id The unique identifier for the Singularity.
   * @param atom Pointer to the associated Atom instance.
   * @param language The language associated with the Atom.
   * @param hook
   */
  Singularity(unsigned id, Atom *atom, Language language, Hook *hook);

  /**
   * @brief Checks if the associated Atom contains any issues.
   *
   * This method evaluates whether the Atom instance linked to the Singularity
   * object has any detectable issues by invoking the `contains_issue` method on
   * the Atom instance. If an issue is detected, the method returns a pointer to
   * the current Singularity object. Otherwise, it throws a runtime exception.
   *
   * @throws std::runtime_error if the associated Atom does not contain any
   * issues.
   * @return A pointer to the current Singularity object if an issue is
   * detected.
   */
  bool check();

private:
  /**
   * @brief Represents the unique issue identifier for the associated Atom.
   *
   * This variable is used to store an identifier that links the Singularity
   * object to a specific issue within the associated Atom instance. It is
   * initialized to 0 by default and is set during the construction of the
   * Singularity object.
   */
  unsigned int issue = 0;
  /**
   * @brief Pointer to the associated Atom instance.
   *
   * This variable holds a pointer to an Atom object associated with the
   * Singularity instance. It is initialized during the Singularity's
   * construction and is used to interact with the Atom for operations such as
   * issue detection. The pointer is also managed and deleted in the Singularity
   * destructor to ensure proper cleanup of resources.
   */
  Atom *a;
  Language lang;
  Hook *h;
  bool isAccepted = false;
};
} // namespace Jah
