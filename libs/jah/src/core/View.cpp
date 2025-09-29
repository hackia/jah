#include "../../include/core/View.hpp"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace Jah;
using namespace std;

namespace {
std::vector<std::string> readIgnorePatterns() {
  std::vector<std::string> patterns;
  static const std::vector<std::string> ignoreFiles = {".gitignore", ".ignore"};

  for (const auto &ignoreFile : ignoreFiles) {
    if (fs::exists(ignoreFile)) {
      std::ifstream file(ignoreFile);
      std::string line;
      while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
          patterns.push_back(line);
        }
      }
    }
  }
  return patterns;
}

bool isIgnored(const fs::path &p) {
  static const auto patterns = readIgnorePatterns();
  const std::string path = p.string();

  for (const auto &pattern : patterns) {
    if (path.find(pattern) != std::string::npos) {
      return true;
    }
  }
  return false;
}

std::string toLower(std::string s) {
  std::ranges::transform(s, s.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return s;
}

std::string extOf(const fs::path &p) {
  auto e = p.extension().string();
  if (!e.empty() && e[0] == '.')
    e.erase(0, 1);
  return toLower(e);
}

bool nameContains(const fs::path &p, const char *needle) {
  const auto lower = toLower(p.filename().string());
  const std::string n = toLower(needle);
  return lower.find(n) != std::string::npos;
}

bool isInDirNamed(const fs::path &p, const char *dirName) {
  const std::string needle = toLower(dirName);
  for (auto it = p.parent_path(); !it.empty(); it = it.parent_path()) {
    if (toLower(it.filename().string()) == needle)
      return true;
  }
  return false;
}

bool isTestFile(const fs::path &p) {
  if (nameContains(p, "test") || nameContains(p, "spec") ||
      nameContains(p, "fixture"))
    return true;
  if (isInDirNamed(p, "test") || isInDirNamed(p, "tests") ||
      isInDirNamed(p, "testing"))
    return true;
  static const char *patterns[] = {"test.cpp", "_test.cpp", ".spec.", ".test.",
                                   ".feature"};
  const std::string fnLower = toLower(p.filename().string());
  return std::ranges::any_of(patterns, [&fnLower](const char *pat) {
    return fnLower.find(pat) != std::string::npos;
  });
}

bool isDesignAsset(const fs::path &p) {
  static const char *extensions[] = {
      "css",  "scss", "sass", "less", "styl", "svg", "png",  "jpg",
      "jpeg", "gif",  "webp", "ico",  "ttf",  "otf", "woff", "woff2"};
  if (const std::string e = extOf(p);
      std::ranges::any_of(extensions, [&e](const char *ex) { return e == ex; }))
    return true;
  return isInDirNamed(p, "assets") || isInDirNamed(p, "design") ||
         isInDirNamed(p, "ui");
}

bool isSourceCode(const fs::path &p) {
  static const char *extensions[] = {
      "cpp",  "hpp",  "c",    "h",      "cc",          "hh",    "ixx",
      "tpp",  "cxx",  "hxx",  "py",     "pyw",         "pyx",   "pxd",
      "pyi",  "java", "kt",   "groovy", "scala",       "js",    "jsx",
      "ts",   "tsx",  "php",  "html",   "htm",         "xhtml", "css",
      "scss", "sass", "rb",   "erb",    "rake",        "slim",  "go",
      "mod",  "rs",   "rlib", "swift",  "swiftmodule", "sh",    "bash",
      "zsh",  "fish", "cs",   "cshtml", "csproj",      "vb",    "lua",
      "pl",   "pm",   "xml",  "json",   "yaml",        "yml",   "toml",
      "ini",  "conf"};

  const std::string e = extOf(p);
  return std::ranges::any_of(
      extensions, [&e](const char *extension) { return e == extension; });
}

bool isDocsOrMgmt(const fs::path &p) {
  static const char *docExtensions[] = {"md", "txt", "rst", "pdf", "csv"};
  if (const std::string e = extOf(p); std::ranges::any_of(
          docExtensions, [&e](const char *ex) { return e == ex; }))
    return true;
  const std::string filename = toLower(p.filename().string());
  return filename == "readme.md" || filename == "license" ||
         isInDirNamed(p, "docs");
}

} // namespace

View::View(const Observer observer) : o(observer) {}

bool View::canSee(const fs::path &p) const {
  switch (o) {
  case Manager:
    return isDocsOrMgmt(p);
  case Developer:
    return isSourceCode(p) && !isTestFile(p);
  case Designer:
    return isDesignAsset(p);
  case Tester:
  case QALead:
  case QA:
    return isTestFile(p);
  case Architect:
  case DatabaseAdmin:
    return isSourceCode(p);
  case ProductOwner:
  case ScrumMaster:
  case TechWriter:
    return isDocsOrMgmt(p);
  case DevOps:
  case SecurityEngineer:
  case SysAdmin:
    return isSourceCode(p) || isTestFile(p);
  case UIDesigner:
  case UXDesigner:
    return isDesignAsset(p);
  case FrontendDev:
    return (isSourceCode(p) && !isTestFile(p)) || isDesignAsset(p);
  case BackendDev:
    return isSourceCode(p) && !isTestFile(p);
  case FullStackDev:
    return isSourceCode(p) || isDesignAsset(p);
  case TeamLead:
    return isSourceCode(p) || isTestFile(p) || isDocsOrMgmt(p);
  }
  return false;
}

std::vector<fs::path> View::filter(const std::vector<fs::path> &paths) const {
  std::vector<fs::path> out;
  for (const auto &path : paths) {
    if (fs::is_directory(path)) {
      if (!isIgnored(path) && canSee(path)) {
        out.push_back(path);
      }
      for (const auto &entry : fs::recursive_directory_iterator(path)) {
        if (!isIgnored(entry.path()) && canSee(entry.path())) {
          out.push_back(entry.path());
        }
      }
    } else if (!isIgnored(path) && canSee(path)) {
      out.push_back(path);
    }
  }
  return out;
}
void View::printTree(const char *str, const char *prefix, int &total) const {
  std::vector<fs::path> entries;
  try {
    for (const auto &entry : fs::directory_iterator(str)) {
      entries.push_back(entry.path());
    }
    std::ranges::sort(entries);
  } catch (const fs::filesystem_error &) {
    return;
  }

  for (size_t i = 0; i < entries.size(); ++i) {
    const auto &entry = entries[i];
    const bool isLast = (i == entries.size() - 1);

    if (isIgnored(entry)) {
      continue;
    }

    if (fs::is_directory(entry)) {
      std::cout << prefix << (isLast ? "└── " : "├── ") << "\033[1;34m"
                << entry.filename().string() << "\033[0m" << std::endl;
      printTree(entry.c_str(), isLast ? "    " : "│   ", total);
    } else if (this->canSee(entry)) {
      total++;
      const auto bytes = fs::file_size(entry);
      string size = (bytes < 1024) ? to_string(bytes) + "B"
                    : (bytes < 1024 * 1024)
                        ? to_string(bytes / 1024) + "KB"
                        : to_string(bytes / (1024 * 1024)) + "MB";

      std::cout << prefix << (isLast ? "└── " : "├── ")
                << entry.filename().string() << " \033[90m(" << size
                << ")\033[0m" << std::endl;
    }
  }
}
int View::ls() const {
  cout << "." << endl;
  try {
    int totalFiles = 0;
    this->printTree(".", "", totalFiles);

    if (totalFiles == 0) {
      cout << "No visible files found for your role." << endl;
    } else {
      cout << "\nTotal: " << totalFiles << " visible files." << endl;
    }
    return 0;
  } catch (const fs::filesystem_error &e) {
    cerr << "Filesystem error: " << e.what() << endl;
    return 1;
  }
}
