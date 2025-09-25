#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace Jah {

/**
 * @brief An enumeration that represents the role of a user within a system.
 *
 * This enum is used to define the type of role (e.g., Manager, Developer)
 * for features like access control or role-based functionality.
 */
enum Observer {
  Manager,
  Developer,
  Designer,
  Tester,
  Architect,
  ProductOwner,
  ScrumMaster,
  QALead,
  TechWriter,
  DevOps,
  SecurityEngineer,
  DatabaseAdmin,
  UIDesigner,
  UXDesigner,
  FrontendDev,
  BackendDev,
  FullStackDev,
  SysAdmin,
  QA,
  TeamLead,
};

/**
 * @class View
 *
 * @brief Represents a filtered view of files and directories, determined by the
 * user role.
 *
 * This class provides functionality to evaluate the visibility of files and
 * directories based on the role of the user (e.g., Manager, Developer,
 * Designer, Tester). It supports filtering a list of paths based on the
 * specified role to only include paths that are relevant and visible for that
 * role.
 */
class View {
  /**
   * Constructs a View object with the specified member role.
   *
   * @param observer The role of the member associated with the view, which
   * determines access permissions.
   */
public:
  explicit View(Observer observer);
  /**
   * Determines whether the given file or directory path is visible to the
   * current view, based on the member role.
   *
   * This method evaluates the file type, directory structure, or naming
   * conventions of the path based on the role of the member (e.g., Manager,
   * Developer, Designer, Tester).
   *
   * @param p The file or directory path to check visibility for.
   * @return true if the file or directory is visible to the current view based
   * on the member role; false otherwise.
   */
  [[nodiscard]] bool canSee(const fs::path &p) const;

  /**
   * Filters a list of file paths, retaining only those that the current view
   * can access. The filtering is based on the role of the member associated
   * with the view.
   *
   * @param paths A vector of file paths to be filtered.
   * @return A vector containing the file paths that are accessible to the
   * current view.
   */
  [[nodiscard]] std::vector<fs::path>
  filter(const std::vector<fs::path> &paths) const;
  /**
   * Represents the role or responsibility of a user interacting with the
   * system. Determines the type of files the user can access or view based on
   * their role.
   */
private:
  Observer o;
};
} // namespace Jah
