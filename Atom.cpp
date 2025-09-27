#include "Atom.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <vector>
using namespace Jah;
using namespace std;

Atom::Atom(string type, string summary, string body, string footer, string id)
    : type(std::move(type)), summary(std::move(summary)),
      footer(std::move(footer)), body(std::move(body)), issue(std::move(id)) {}

string Atom::getType() const { return type; }
string Atom::getSummary() const { return summary; }
string Atom::getBody() const { return body; }
string Atom::getFooter() const { return footer; }

int Atom::save() const {
  const char *host = getenv("PG_HOST") ? getenv("PG_HOST") : "localhost";
  const char *port = getenv("PG_PORT") ? getenv("PG_PORT") : "5432";
  const char *dbname = getenv("PG_DBNAME") ? getenv("PG_DBNAME") : "jah";
  const char *user = getenv("PG_USER") ? getenv("PG_USER") : "postgres";
  const char *password =
      getenv("PG_PASSWORD") ? getenv("PG_PASSWORD") : "postgres";

  const string connStr = "host=" + string(host) + " port=" + string(port) +
                         " dbname=" + string(dbname) + " user=" + string(user) +
                         " password=" + string(password);

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
int Atom::log(const int limit) {
  const char *host = getenv("PG_HOST") ? getenv("PG_HOST") : "localhost";
  const char *port = getenv("PG_PORT") ? getenv("PG_PORT") : "5432";
  const char *dbname = getenv("PG_DBNAME") ? getenv("PG_DBNAME") : "jah";
  const char *user = getenv("PG_USER") ? getenv("PG_USER") : "postgres";
  const char *password =
      getenv("PG_PASSWORD") ? getenv("PG_PASSWORD") : "postgres";

  const string connStr = "host=" + string(host) + " port=" + string(port) +
                         " dbname=" + string(dbname) + " user=" + string(user) +
                         " password=" + string(password);

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
int Atom::accepted() { return ATOM_ACCEPTED; }
int Atom::refused() { return ATOM_REFUSED; }
