#pragma once
#include <string>
using namespace std;

namespace Jah {
/**
 * @class Config
 * @brief Represents the configuration for database connection details.
 *
 * The Config class serves as a container for essential database connection
 * parameters such as username, password, host, port, and the database name. It
 * provides accessor methods to retrieve these values with default fallbacks
 * when necessary.
 *
 */
class Config {
public:

  Config(string username, string password, string host,
         string port, string base);
  [[nodiscard]] string getUsername() const;
  [[nodiscard]] string getPassword() const;
  [[nodiscard]] string getHost() const;
  [[nodiscard]] string getPort() const;
  [[nodiscard]] string getDatabase() const;
private:
  string username;
  string password;
  string host;
  string port;
  string base;
};
} // namespace Jah