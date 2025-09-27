#include "Model.hpp"
#include <algorithm>
#include <libpq-fe.h>
#include <string>
#include <vector>
using namespace Jah;
using namespace std;

Model::Model(const Config &config)
    : username(config.getUsername()), password(config.getPassword()),
      host(config.getHost()), port(config.getPort()),
      base(config.getDatabase()) {}

bool Model::execute(const string &sql) const {
  PGconn *conn = PQconnectdb(this->getDbUrl().c_str());
  if (PQstatus(conn) != CONNECTION_OK) {
    PQfinish(conn);
    return false;
  }
  PGresult *res = PQexec(conn, sql.c_str());
  const bool success = PQresultStatus(res) == PGRES_COMMAND_OK;
  PQclear(res);
  PQfinish(conn);
  return success;
}
string Model::getDbUrl() const {
  string url;
  url.reserve(100);
  url.append(username)
      .append(":")
      .append(password)
      .append("@")
      .append(host)
      .append(":")
      .append(port)
      .append("/")
      .append(base)
      .insert(0, "postgresql://");
  return url;
}
bool Model::Down() const {
  vector<bool> status;
  for (const auto &s : DOWN_TABLES) {
    status.push_back(this->execute(s));
  }
  if (!ranges::all_of(status, [](const bool s) { return s; })) {
    return false;
  }
  return true;
}
bool Model::up() const {
  vector<bool> status;
  for (const auto &s : UP_TABLES) {
    status.push_back(this->execute(s));
  }
  if (!ranges::all_of(status, [](const bool s) { return s; })) {
    return false;
  }
  return true;
}