#include "Ji.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace Jah;
using namespace std;

int Ji::init() {
  const fs::path repo_dir = JAH_DIRECTORY_NAME;
  const fs::path db_file = JAH_DATABASE;

  // If the database already exists, consider the repo initialized
  if (fs::exists(db_file)) {
    cerr << "Jah database already exists" << endl;
    return EXIT_FAILURE;
  }

  // Ensure the repository directory exists (create it if missing)
  try {
    if (!fs::exists(repo_dir)) {
      fs::create_directory(repo_dir);
    }
  } catch (const fs::filesystem_error &e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }

  // Create and initialize the single database file that will store all objects
  ofstream db_stream(db_file, ios::binary | ios::trunc);
  if (!db_stream.is_open()) {
    cerr << "Failed to write the " << db_file << " file" << endl;
    return EXIT_FAILURE;
  }
  db_stream.write(reinterpret_cast<const char *>(MAGIC.data()), MAGIC.size());
  db_stream.write(reinterpret_cast<const char *>(&VERSION), sizeof(VERSION));
  constexpr uint64_t object_count = 0;
  db_stream.write(reinterpret_cast<const char *>(&object_count), sizeof(object_count));
  db_stream.flush();
  db_stream.close();

  if (!db_stream) {
    cerr << "Sorry an error has been encountered on db file creation." << endl;
    return EXIT_FAILURE;
  }
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
