#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

namespace Jah {
constexpr array<uint8_t, 6> MAGIC = {0x4A, 0x61, 0x6A, 0x61, 0x37, 0x37};
constexpr uint32_t VERSION = 0x00000001;
const path JAH_DIRECTORY_NAME = ".jah";
const path JAH_DATABASE = ".jah/ji.jah";

enum class ObjectType : uint8_t {
  Commit = 0x01,
  Tree = 0x02,
  Blob = 0x03,
  Runner = 0x04,
  TimeEntry = 0x05,
};

class Ji {
public:
  static int init(const string &hostname, const string &port, const string &dbname, const string &user,
                  const string &password, const string &username, const string &email);
  static bool isValid();
};
} // namespace Jah
