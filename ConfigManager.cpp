#include "ConfigManager.hpp"
#include <iostream>
#include <stdexcept>

using namespace Jah;

ConfigManager::ConfigManager(const std::string& db_path) : db(nullptr) {
  if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
    throw std::runtime_error("Impossible d'ouvrir la base de données de configuration: " + std::string(sqlite3_errmsg(db)));
  }
  initializeDatabase();
}

ConfigManager::~ConfigManager() {
  if (db) {
    sqlite3_close(db);
  }
}

void ConfigManager::initializeDatabase() {
  const char* sql = "CREATE TABLE IF NOT EXISTS settings (key TEXT PRIMARY KEY NOT NULL, value TEXT NOT NULL);";
  char* errMsg = nullptr;
  if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
    std::string err = "Erreur SQL lors de la création de la table settings: " + std::string(errMsg);
    sqlite3_free(errMsg);
    throw std::runtime_error(err);
  }
}

void ConfigManager::set(const std::string& key, const std::string& value) {
  // Utilise une requête préparée pour insérer ou remplacer une clé
  sqlite3_stmt* stmt;
  const char* sql = "INSERT OR REPLACE INTO settings (key, value) VALUES (?, ?);";

  sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
  sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    // Gérer l'erreur
  }
  sqlite3_finalize(stmt);
}

std::string ConfigManager::get(const std::string& key) {
  sqlite3_stmt* stmt;
  const char* sql = "SELECT value FROM settings WHERE key = ?;";

  sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
  sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);

  std::string result = "";
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  }

  sqlite3_finalize(stmt);
  return result;
}