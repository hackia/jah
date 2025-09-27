#include "Ji.hpp"
#include "Atom.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <limits>
#include <ostream>
#include <vector>
using namespace Jah;
using namespace std;
using namespace std::filesystem;
/**
 * @brief SQL script defining the schema for a set of tables used in the
 * application.
 *
 * The create_tables SQL command creates the following tables:
 *
 * 1. Commits:
 *    - Stores information about commits, including a unique hash, message,
 * author, and timestamp.
 *    - Primary Key: id
 *
 * 2. Trees:
 *    - Represents directory trees associated with commits.
 *    - Contains a unique hash, a path, and a reference to the associated
 * commit.
 *    - Primary Key: id
 *    - Foreign Key: commit_id references the commits table. Rows are deleted if
 * the associated commit is deleted (ON DELETE CASCADE).
 *
 * 3. Blobs:
 *    - Represents file content stored as binary data.
 *    - Contains a unique hash, file name, and a reference to the associated
 * tree.
 *    - Primary Key: id
 *    - Foreign Key: tree_id references the trees table. Rows are deleted if the
 * associated tree is deleted (ON DELETE CASCADE).
 *
 * 4. Runners:
 *    - Tracks processing or pipelines associated with a commit.
 *    - Contains a status, start time, and an optional end time.
 *    - Primary Key: id
 *    - Foreign Key: commit_id references the commits table. Rows are deleted if
 * the associated commit is deleted (ON DELETE CASCADE).
 *
 * 5. Time_entries:
 *    - Logs time entries for runners, including a duration and description.
 *    - The duration must be greater than zero.
 *    - Primary Key: id
 *    - Foreign Key: runner_id references the runners table. Rows are deleted if
 * the associated runner is deleted (ON DELETE CASCADE).
 *
 * 6. Discussions:
 *    - Represents discussions or comments on specific commits.
 *    - Includes the author, message, and timestamp.
 *    - Primary Key: id
 *    - Foreign Key: commit_id references the commits table. Rows are deleted if
 * the associated commit is deleted (ON DELETE CASCADE).
 *
 * 7. Tickets:
 *    - Tracks issues, tasks, or tickets with an associated title, description,
 * status, assignee, and creation timestamp.
 *    - Contains an optional reference to a specific commit.
 *    - Primary Key: id
 *    - Foreign Key: commit_id references the commits table. If the associated
 * commit is deleted, the reference to it is set to NULL (ON DELETE SET NULL).
 */
const auto create_tables =
    "CREATE TABLE commits ("
    "  id SERIAL PRIMARY KEY,"
    "  hash VARCHAR(40) NOT NULL UNIQUE,"
    "  message TEXT NOT NULL,"
    "  author VARCHAR(100) NOT NULL,"
    "  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
    ");"
    "CREATE TABLE trees ("
    "  id SERIAL PRIMARY KEY,"
    "  hash VARCHAR(40) NOT NULL UNIQUE,"
    "  commit_id INTEGER REFERENCES commits(id) ON DELETE CASCADE,"
    "  path TEXT NOT NULL"
    ");"
    "CREATE TABLE blobs ("
    "  id SERIAL PRIMARY KEY,"
    "  hash VARCHAR(40) NOT NULL UNIQUE,"
    "  tree_id INTEGER REFERENCES trees(id) ON DELETE CASCADE,"
    "  name TEXT NOT NULL,"
    "  content BYTEA NOT NULL"
    ");"
    "CREATE TABLE runners ("
    "  id SERIAL PRIMARY KEY,"
    "  commit_id INTEGER REFERENCES commits(id) ON DELETE CASCADE,"
    "  status VARCHAR(20) NOT NULL DEFAULT 'pending',"
    "  start_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
    "  end_time TIMESTAMP"
    ");"
    "CREATE TABLE time_entries ("
    "  id SERIAL PRIMARY KEY,"
    "  runner_id INTEGER REFERENCES runners(id) ON DELETE CASCADE,"
    "  duration INTEGER NOT NULL CHECK (duration > 0),"
    "  description TEXT NOT NULL"
    ");"
    "CREATE TABLE discussions ("
    "  id SERIAL PRIMARY KEY,"
    "  commit_id INTEGER REFERENCES commits(id) ON DELETE CASCADE,"
    "  author VARCHAR(100) NOT NULL,"
    "  message TEXT NOT NULL,"
    "  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
    ");"
    "CREATE TABLE tickets ("
    "  id SERIAL PRIMARY KEY,"
    "  title VARCHAR(200) NOT NULL,"
    "  description TEXT NOT NULL,"
    "  status VARCHAR(20) NOT NULL DEFAULT 'open',"
    "  assignee VARCHAR(100),"
    "  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
    "  commit_id INTEGER REFERENCES commits(id) ON DELETE SET NULL"
    ");";

int Ji::init(const string &hostname, const string &port, const string &dbname,
             const string &user, const string &password, const string &username,
             const string &email) {
  const path repo_dir = JAH_DIRECTORY_NAME;
  if (exists(".env")) {
    remove(".env");
  }

  // Now you can safely use these std::string variables.
  std::cout << "\n--- Configuration Summary ---" << std::endl;
  std::cout << "Hostname  : " << hostname << std::endl;
  std::cout << "Port      : " << port << std::endl;
  std::cout << "Database  : " << dbname << std::endl;
  std::cout << "user      : " << user << std::endl;
  std::cout << "password  : " << password << std::endl;
  std::cout << "developer : " << username << std::endl;
  std::cout << "email     : " << email << std::endl;

  try {
    ofstream envFile(".env");
    envFile << "PG_HOST=" << hostname << endl
            << "PG_PORT=" << port << endl
            << "PG_DBNAME=" << dbname << endl
            << "PG_USER=" << user << endl
            << "PG_PASSWORD=" << password << endl
            << "USERNAME=" << username << endl
            << "EMAIL=" << email << endl;
    envFile.close();
  } catch (filesystem_error &err) {
    cout << err.what() << endl;
    return err.code().value();
  }

  const string databaseConnectionSettings =
      "host=" + string(hostname) + " port=" + string(port) +
      " dbname=" + string(dbname) + " user=" + string(user) +
      " password=" + string(password);

  PGconn *conn = PQconnectdb(databaseConnectionSettings.c_str());

  if (PQstatus(conn) != CONNECTION_OK) {
    cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
    PQfinish(conn);
    return EXIT_FAILURE;
  }

  // Start transaction
  PGresult *res = PQexec(conn, "BEGIN");
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cerr << "BEGIN command failed: " << PQerrorMessage(conn) << endl;
    PQclear(res);
    PQfinish(conn);
    return EXIT_FAILURE;
  }
  PQclear(res);

  // Ensure the repository directory exists (create it if missing)
  try {
    if (!exists(repo_dir)) {
      create_directory(repo_dir);
    }
  } catch (const filesystem_error &e) {
    PQexec(conn, "ROLLBACK");
    PQfinish(conn);
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
  res = PQexec(conn, create_tables);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cerr << "Table creation failed: " << PQerrorMessage(conn) << endl;
    PQclear(res);
    PQexec(conn, "ROLLBACK");
    PQfinish(conn);
    return EXIT_FAILURE;
  }
  PQclear(res);

  // Commit transaction
  res = PQexec(conn, "COMMIT");
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cerr << "COMMIT failed: " << PQerrorMessage(conn) << endl;
    PQclear(res);
    PQexec(conn, "ROLLBACK");
    PQfinish(conn);
    return EXIT_FAILURE;
  }
  PQclear(res);
  PQfinish(conn);

  cout << "Initialized Jah object database successfully" << endl;
  return EXIT_SUCCESS;
}

bool Ji::isValid() {
  ifstream file(JAH_DATABASE, ios::binary);
  array<uint8_t, 6> magic_from_file{};

  file.read(reinterpret_cast<char *>(magic_from_file.data()),
            magic_from_file.size());

  if (!file) {
    return false;
  }
  if (memcmp(magic_from_file.data(), MAGIC.data(), magic_from_file.size()) !=
      0) {
    cerr << "No detecting jah repository" << endl;
    return false;
  }
  return true;
}
