#include "Config.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <libpq-fe.h>
#include <sqlite3.h>
using namespace Jah;
using namespace std;
namespace fs = std::filesystem;

Config::Config() {
  sqlite3 *db;
  char *errMsg = nullptr;
  int rc = sqlite3_open(".jah/jah.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  }

  const auto *sql = "SELECT author, email, username, password, host, port, "
                    "database FROM config LIMIT 1;";

  auto callback = [](void *data, const int argc, char **argv,
                     char **colNames) -> int {
    auto *config = static_cast<Config *>(data);
    for (int i = 0; i < argc; i++) {
      if (argv[i]) {
        if (strcmp(colNames[i], "author") == 0)
          config->author = argv[i];
        else if (strcmp(colNames[i], "email") == 0)
          config->email = argv[i];
        else if (strcmp(colNames[i], "username") == 0)
          config->username = argv[i];
        else if (strcmp(colNames[i], "password") == 0)
          config->password = argv[i];
        else if (strcmp(colNames[i], "host") == 0)
          config->host = argv[i];
        else if (strcmp(colNames[i], "port") == 0)
          config->port = argv[i];
        else if (strcmp(colNames[i], "database") == 0)
          config->base = argv[i];
      }
    }
    return 0;
  };
  rc = sqlite3_exec(db, sql, callback, this, &errMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  }

  sqlite3_close(db);
}
int Config::initialize() {

  if (!fs::exists(".jah")) {
    filesystem::create_directory(".jah");
  }
  sqlite3 *db;
  char *errMsg = nullptr;
  int rc = sqlite3_open(".jah/jah.db", &db);

  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return 1;
  }
  const auto *sql = "CREATE TABLE IF NOT EXISTS config ("
                    "author TEXT, email TEXT, username TEXT, "
                    "password TEXT, host TEXT, port TEXT, database TEXT);";

  rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << errMsg << std::endl;
    sqlite3_free(errMsg);
    sqlite3_close(db);
    return 1;
  }
  const PGconn *pgConn = nullptr;
  do {
    author.clear();
    email.clear();
    username.clear();
    password.clear();
    host.clear();
    port.clear();
    base.clear();
    std::cout << "Enter author name: ";
    std::getline(std::cin, author);

    std::cout << "Enter email: ";
    std::getline(std::cin, email);

    std::cout << "Enter database username [postgres]: ";
    std::getline(std::cin, username);
    if (username.empty())
      username = "postgres";

    std::cout << "Enter database password: ";
    std::getline(std::cin >> std::ws, password);

    std::cout << "Enter database host [localhost]: ";
    std::getline(std::cin, host);
    if (host.empty())
      host = "localhost";

    std::cout << "Enter database port [5432]: ";
    std::getline(std::cin, port);
    if (port.empty())
      port = "5432";

    std::cout << "Enter database name: ";
    std::getline(std::cin, base);

    string conninfo = "host=" + host + " port=" + port + " dbname=" + base +
                      " user=" + username + " password=" + password;

    pgConn = PQconnectdb(conninfo.c_str());
  } while (PQstatus(pgConn) != CONNECTION_OK);

  sql = "INSERT INTO config (author, email, username, password, host, port, "
        "database) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    sqlite3_close(db);
    return 1;
  }
  sqlite3_bind_text(stmt, 1, author.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, username.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, password.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, host.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 6, port.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 7, base.c_str(), -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;
}

string Config::getAuthor() const {
  if (author.empty()) {
    return "";
  }
  return author;
}
string Config::getEmail() const {
  if (email.empty()) {
    return "";
  }
  return email;
}
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
    return "";
  }
  return base;
}