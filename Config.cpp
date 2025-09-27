#include "Config.hpp"

#include <utility>

using namespace Jah;

Config::Config(string username, string password, string host, string port,
               string base)
    : username(std::move(username)), password(std::move(password)),
      host(std::move(host)), port(std::move(port)), base(std::move(base)) {}

string Config::getUsername() const {
  if (username.empty()) {
    return "postgres";
  }
  return username;
}
string Config::getPassword() const {
  if (password.empty()) {
    return "";
  }
  return password;
}
string Config::getHost() const {
  if (host.empty()) {
    return "localhost";
  }
  return host;
}
string Config::getPort() const {
  if (port.empty()) {
    return "5432";
  }
  return port;
}
string Config::getDatabase() const {
  if (base.empty()) {
    return "jah";
  }
  return base;
}