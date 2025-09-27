#pragma once
#include "Config.hpp"
#include <string>
using namespace std;
namespace Jah {
/**
 * @brief Represents an Atom object with distinct properties such as type,
 *        summary, body, and footer.
 *
 * This class provides functionality to manage and access the properties
 * of an Atom instance, including its type, summary, main body, and footer.
 * It also includes methods for status checks and issue-related functionality.
 */
class Atom {
public:
  Atom(string type, string summary, string body, string note,string footer, string id);
  /**
   * @brief Retrieves the type associated with the Atom instance.
   *
   * The type specifies the category or classification of the Atom object.
   *
   * @return A string representing the type of the Atom.
   */
  [[nodiscard]] string getType() const;
  /**
   * @brief Retrieves the summary associated with the Atom instance.
   *
   * The summary provides a concise description or key information related
   * to the Atom object.
   *
   * @return A string representing the summary of the Atom.
   */
  [[nodiscard]] string getSummary() const;
  /**
   * @brief Retrieves the body text associated with the Atom instance.
   *
   * The body provides the main content or detailed information encapsulated
   * by the Atom object.
   *
   * @return A string representing the body of the Atom.
   */
  [[nodiscard]] string getBody() const;
  /**
   * @brief Retrieves the footer associated with the Atom instance.
   *
   * The footer provides additional information or context related to the Atom
   * object.
   *
   * @return A string representing the footer of the Atom.
   */
  [[nodiscard]] string getFooter() const;
  /**
   * @brief Saves the Atom object's state to a file within a specific directory.
   *
   * This method writes the type, summary, body, and footer of the Atom instance
   * to a binary file located at ".jah/ji.jah". The save operation only proceeds
   * if the required directory and file exist. If the save is successful, a
   * confirmation is displayed; otherwise, an error message is shown.
   *
   * @return An integer indicating the exit status of the save operation.
   *         Returns EXIT_SUCCESS if the data is saved successfully, or
   *         EXIT_FAILURE if an error occurs or prerequisites are not met.
   */
  [[nodiscard]] int save(const Config &config) const;
  static int log(int limit, const Config &config);
  /**
   * @brief Displays the commit history log in reverse order.
   *
   * This static method reads a commit history file, retrieves its content,
   * and prints each commit in reverse chronological order to the console.
   * If the file cannot be opened, an error message is displayed, and the method
   * terminates with a failure code.
   *
   * @return An integer indicating the exit status of the operation.
   *         Returns EXIT_SUCCESS if the log is displayed successfully,
   *         or EXIT_FAILURE if an error occurs.
   */

  /**
   * @brief Retrieves the status code indicating that the Atom instance is
   * accepted.
   *
   * This method returns a predefined constant that symbolizes an accepted state
   * for the Atom entity.
   *
   * @return An integer value representing the acceptance state of the Atom.
   */
  static int accepted();

  /**
   * @brief Retrieves the status code indicating that the Atom instance is
   * refused.
   *
   * This method returns a predefined constant that represents a refused state
   * for the Atom entity.
   *
   * @return An integer value representing the refusal state of the Atom.
   */
  static int refused();

private:
  /**
   * @brief Specifies the classification or category of the Atom instance.
   *
   * This variable holds the type associated with the Atom object, providing
   * a means to identify or organize it under a specific classification.
   */
  string type;
  /**
   * @brief Provides a concise description or key information related to
   * the Atom instance.
   *
   * This variable stores the summary of the Atom, which represents a
   * brief and high-level description of the encapsulated entity.
   */
  string summary;
  /**
   * @brief Provides additional context or information about the Atom instance.
   *
   * The footer is a supplementary text element associated with an Atom
   * instance, often used to store additional comments, metadata, or contextual
   * details. It can also be used for specific attributes like issue
   * tracking or other appendices.
   */
  string footer;
  /**
   * @brief Represents the primary content or main information encapsulated by
   * the Atom instance.
   *
   * This variable contains the detailed content or the essential body text
   * associated with the Atom. It is a key part of the Atom object, used
   * to store and manage the main textual data for the entity.
   */
  string body;

  string issue;

  string note;
};
} // namespace Jah
