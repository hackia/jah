#pragma once
#include "Config.hpp"
#include <string>
#include <vector>
using namespace std;

const auto UP_COMMITS =
    "CREATE TABLE IF NOT EXISTS commits (id SERIAL PRIMARY KEY, hash "
    "VARCHAR(40) NOT NULL UNIQUE, message TEXT NOT NULL,author VARCHAR(100) "
    "NOT NULL, "
    "note TEXT NOT NULL , "
    "issue TEXT NOT NULL , "
    "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP;";
const auto DOWN_COMMITTED_TABLE = "DROP TABLE IF EXISTS commits;";
const auto UP_TREE_TABLE =
    "CREATE TABLE IF NOT EXISTS trees (id SERIAL PRIMARY KEY, hash VARCHAR(40) "
    "NOT NULL UNIQUE, commit_id INTEGER REFERENCES commits(id) ON DELETE "
    "CASCADE, path TEXT NOT NULL);";
const auto DOWN_TREE_TABLE = "DROP TABLE IF EXISTS trees;";
const auto UP_BLOB_TABLE =
    "CREATE TABLE IF NOT EXISTS blobs (id SERIAL PRIMARY KEY, hash VARCHAR(40) "
    "NOT NULL UNIQUE, tree_id INTEGER REFERENCES trees(id) ON DELETE CASCADE, "
    "name TEXT NOT NULL, content BYTEA NOT NULL);";
const auto DOWN_BLOB_TABLE = "DROP TABLE IF EXISTS blobs;";
const auto UP_RUNNER_TABLE =
    "CREATE TABLE IF NOT EXISTS runners (id SERIAL PRIMARY KEY, commit_id "
    "INTEGER REFERENCES commits(id) ON DELETE CASCADE, status VARCHAR(20) NOT "
    "NULL DEFAULT 'pending', start_time TIMESTAMP NOT NULL DEFAULT "
    "CURRENT_TIMESTAMP, end_time TIMESTAMP);";
const auto DOWN_RUNNER_TABLE = "DROP TABLE IF EXISTS runners;";
const auto UP_TIME_ENTRIES =
    "CREATE TABLE IF NOT EXISTS time_entries ("
    "  id SERIAL PRIMARY KEY,"
    "  runner_id INTEGER REFERENCES runners(id) ON DELETE CASCADE,"
    "  duration INTEGER NOT NULL CHECK (duration > 0),"
    "  description TEXT NOT NULL"
    ");";
const auto DOWN_TIME_ENTRIES = "DROP TABLE IF EXISTS time_entries;";
const auto UP_DISCUSSIONS =
    "CREATE TABLE IF NOT EXISTS discussions ("
    "  id SERIAL PRIMARY KEY,"
    "  commit_id INTEGER REFERENCES commits(id) ON DELETE CASCADE,"
    "  author VARCHAR(100) NOT NULL,"
    "  message TEXT NOT NULL,"
    "  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
    ");";
const auto DOWN_DISCUSSIONS = "DROP TABLE IF EXISTS discussions;";
const auto UP_TICKETS =
    "CREATE TABLE IF NOT EXISTS tickets ("
    "  id SERIAL PRIMARY KEY,"
    "  title VARCHAR(200) NOT NULL,"
    "  description TEXT NOT NULL,"
    "  status VARCHAR(20) NOT NULL DEFAULT 'open',"
    "  assignee VARCHAR(100),"
    "  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
    "  commit_id INTEGER REFERENCES commits(id) ON DELETE SET NULL"
    ");";

const auto DOWN_TICKETS = "DROP TABLE IF EXISTS tickets;";
const vector<string> UP_TABLES = {
    UP_COMMITS,
    UP_TREE_TABLE,
    UP_BLOB_TABLE,
};

const vector<string> DOWN_TABLES = {
    DOWN_COMMITTED_TABLE, DOWN_TREE_TABLE,  DOWN_BLOB_TABLE, DOWN_RUNNER_TABLE,
    DOWN_TIME_ENTRIES,    DOWN_DISCUSSIONS, DOWN_TICKETS,
};

namespace Jah {
class Model {
public:
  explicit Model(const Config &config);
  [[nodiscard]] bool execute(const string &sql) const;
  [[nodiscard]] string getDbUrl() const;
  [[nodiscard]] bool up() const;
  [[nodiscard]] bool Down() const;

private:
  string username;
  string password;
  string host;
  string port;
  string base;
};
} // namespace Jah