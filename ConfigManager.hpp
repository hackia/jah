#pragma once
#include <string>
#include "sqlite3.h"

namespace Jah {

class ConfigManager {
public:
  // Le constructeur ouvre la base de données et initialise la table si besoin
  explicit ConfigManager(const std::string& db_path);
  // Le destructeur ferme la connexion proprement
  ~ConfigManager();

  // Récupère une valeur de configuration
  std::string get(const std::string& key);

  // Définit une valeur de configuration
  void set(const std::string& key, const std::string& value);

private:
  sqlite3* db; // Le pointeur vers notre base de données
  void initializeDatabase(); // Méthode privée pour créer la table
};

} // namespace Jah