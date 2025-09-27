#include "Atom.hpp"

#include <cstring>
#include <filesystem>
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <vector>
using namespace Jah;
using namespace std;

const auto JAH_USERNAME = getenv("JAH_USERNAME");
const auto JAH_EMAIL = getenv("JAH_EMAIL");

Atom::Atom(string type, string summary, string body, string note, string footer,
           string id)
    : type(std::move(type)), summary(std::move(summary)), footer(std::move(footer)),
      body(std::move(body)), issue(std::move(id)), note(std::move(note)) {}

string Atom::getType() const { return type; }
string Atom::getSummary() const { return summary; }
string Atom::getBody() const { return body; }
string Atom::getFooter() const { return footer; }

int Atom::save(const Config &config) const {
  const string connStr = "host=" + config.pg_host + " port=" + config.pg_port +
                         " dbname=" + config.pg_dbname +
                         " user=" + config.pg_user +
                         " password=" + config.pg_password;
  PGconn *conn = PQconnectdb(connStr.c_str());
  if (PQstatus(conn) != CONNECTION_OK) {
    cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
    PQfinish(conn);
    return EXIT_FAILURE;
  }
  const string message = "[" + to_string(std::time(nullptr)) + "] " +
                         "(author: " + string(JAH_USERNAME) + " <" +
                         string(JAH_EMAIL) + ">) " + type + ": " + summary +
                         "\n\n" + body + "\n\n" + footer;

  const string query = "INSERT INTO commits (message, author) VALUES ($1, $2)";
  const char *paramValues[2] = {message.c_str(), JAH_USERNAME};
  const int paramLengths[2] = {static_cast<int>(message.length()),
                               static_cast<int>(strlen(JAH_USERNAME))};
  constexpr int paramFormats[2] = {0, 0};

  PGresult *res = PQexecParams(conn, query.c_str(), 2, nullptr, paramValues,
                               paramLengths, paramFormats, 0);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cerr << "Insert failed: " << PQerrorMessage(conn) << endl;
    PQclear(res);
    PQfinish(conn);
    return EXIT_FAILURE;
  }

  PQclear(res);
  PQfinish(conn);
  cout << "commited" << endl;
  return EXIT_SUCCESS;
}
int Atom::log(const int limit, const Config &config) {
  const string connStr = "host=" + config.pg_host + " port=" + config.pg_port +
                         " dbname=" + config.pg_dbname +
                         " user=" + config.pg_user +
                         " password=" + config.pg_password;

  PGconn *conn = PQconnectdb(connStr.c_str());
  if (PQstatus(conn) != CONNECTION_OK) {
    cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
    PQfinish(conn);
    return EXIT_FAILURE;
  }

  string query = "SELECT message FROM commits ORDER BY timestamp DESC";
  if (limit > 0) {
    query += " LIMIT " + to_string(limit);
  }

  PGresult *res = PQexec(conn, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    cerr << "Query failed: " << PQerrorMessage(conn) << endl;
    PQclear(res);
    PQfinish(conn);
    return EXIT_FAILURE;
  }

  const int rows = PQntuples(res);
  for (int i = 0; i < rows; i++) {
    cout << PQgetvalue(res, i, 0) << endl;
  }

  PQclear(res);
  PQfinish(conn);
  return EXIT_SUCCESS;
}
int Atom::accepted() { return 0; }
int Atom::refused() { return 1; }
