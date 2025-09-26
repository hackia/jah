#pragma once

namespace Jah {
/**
 * @enum Feat
 * @brief Enum representing various types of feature changes made within a
 * system or project.
 *
 * Each value in this enum describes a specific category of change or update
 * applied to a feature, highlighting its nature and degree of modification.
 *
 * - None: Indicates no changes to features.
 * - Added: A new feature has been introduced.
 * - Modified: An existing feature has been altered or adjusted.
 * - Removed: A feature has been deleted or removed from the system.
 * - Deprecated: A feature is marked as outdated and subject to removal in
 * future updates.
 * - Fixed: A bug within an existing feature has been resolved.
 * - Enhanced: Improvements or upgrades have been made to an existing feature.
 * - Refactored: Code has been restructured without changing the feature's
 * behavior.
 */
enum Feat {
  None,
  Added,
  Modified,
  Removed,
  Deprecated,
  Fixed,
  Enhanced,
  Refactored
};
/**
 * @enum BreakingChanges
 * @brief Enum representing different categories of breaking changes that may
 * occur within a system or project.
 *
 * Each value in this enum describes a specific area of change that can have
 * a significant impact on functionality, compatibility, or dependencies. These
 * changes often require careful consideration and updates by users or
 * developers.
 *
 * - Database: Breaking changes affecting the database structure, schema, or
 * functionality.
 * - Packages: Changes to included packages or dependencies that result in
 * breaking functionality.
 * - Authentication: Changes in authentication mechanisms that disrupt existing
 * authentication processes.
 * - APIEndpoints: Modifications to API endpoints, such as removal or signature
 * changes.
 * - FileSystem: File system-level changes that can impact file operations or
 * structure.
 * - Networking: Breaking changes related to networking protocols or
 * configurations.
 * - Security: Changes that impact security mechanisms or standards.
 * - DataStructures: Modifications to the underlying data structures causing
 * incompatibility.
 * - Protocols: Changes to communication or interaction protocols.
 * - Dependencies: Alterations or updates to dependencies that may have
 * downstream effects.
 */
enum BreakingChanges {
  Database,
  Packages,
  Authentication,
  APIEndpoints,
  FileSystem,
  Networking,
  Security,
  DataStructures,
  Protocols,
  Dependencies
};
/**
 * @class Features
 * @brief Represents a feature and its associated breaking change in a system.
 *
 * The Features class encapsulates a combination of a feature modification type
 * and a breaking change category. It provides a structured way to manage and
 * track the relationship between feature updates and their impact on the
 * system.
 *
 * This class is designed to support the classification of both functional
 * modifications and the potential breaking changes they introduce.
 *
 * @note This class is constructed using two enumerations:
 * - BreakingChanges: Enum defining categories of breaking changes.
 * - Feat: Enum describing types of feature modifications.
 */
class Features {
public:
  Features();
};
} // namespace Jah