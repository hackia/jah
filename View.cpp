#include "View.hpp"

#include <algorithm>
#include <cctype>
#include <string>

using namespace Jah;

namespace {

std::string toLower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
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
  const std::string e = extOf(p);
  if (nameContains(p, "test") || nameContains(p, "spec") ||
      nameContains(p, "fixture"))
    return true;
  if (isInDirNamed(p, "test") || isInDirNamed(p, "tests") ||
      isInDirNamed(p, "testing"))
    return true;
  // Common test file extensions/patterns
  static const char *patterns[] = {"test.cpp", "_test.cpp", ".spec.", ".test.",
                                   ".feature"};
  const std::string fnLower = toLower(p.filename().string());
  for (const auto pat : patterns) {
    if (fnLower.find(pat) != std::string::npos)
      return true;
  }
  (void)e;
  return false;
}

bool isDesignAsset(const fs::path &p) {
  static const char *extensions[] = {
      "css",  "scss", "sass", "less", "styl", "svg", "png",  "jpg",
      "jpeg", "gif",  "webp", "ico",  "ttf",  "otf", "woff", "woff2"};
  const std::string e = extOf(p);
  for (const auto ex : extensions)
    if (e == ex)
      return true;
  if (isInDirNamed(p, "assets") || isInDirNamed(p, "design") ||
      isInDirNamed(p, "ui"))
    return true;
  return false;
}

bool isSourceCode(const fs::path &p) {
  static const char *extensions[] = {
      // C/C++
      "cpp", "hpp", "c", "h", "cc", "hh", "ixx", "tpp", "cxx", "hxx",
      // Python
      "py", "pyw", "pyx", "pxd", "pyi",
      // Java
      "java", "kt", "groovy", "scala",
      // JavaScript/TypeScript
      "js", "jsx", "ts", "tsx",
      // Web
      "php", "html", "htm", "xhtml", "css", "scss", "sass",
      // Ruby
      "rb", "erb", "rake", "slim",
      // Go
      "go", "mod",
      // Rust
      "rs", "rlib",
      // Swift
      "swift", "swiftmodule",
      // Shell scripts
      "sh", "bash", "zsh", "fish",
      // C#
      "cs", "cshtml", "csproj", "vb",
      // Lua
      "lua",
      // Perl
      "pl", "pm",
      // Others
      "xml", "json", "yaml", "yml", "toml", "ini", "conf"};

  const std::string e = extOf(p);
  for (const char *extension : extensions) {
    if (e == extension) {
      return true;
    }
  }
  return false;
}

bool isDocsOrMgmt(const fs::path &p) {
  static const char *docExtensions[] = {"md", "txt", "rst", "pdf", "csv"};
  const std::string e = extOf(p);
  for (const auto ex : docExtensions)
    if (e == ex)
      return true;
  if (toLower(p.filename().string()) == std::string("readme.md"))
    return true;
  if (toLower(p.filename().string()) == std::string("license"))
    return true;
  if (isInDirNamed(p, "docs"))
    return true;
  return false;
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
    return isTestFile(p);
  case Architect:
    return isSourceCode(p);
  case ProductOwner:
  case ScrumMaster:
    return isDocsOrMgmt(p);
  case QALead:
    return isTestFile(p);
  case TechWriter:
    return isDocsOrMgmt(p);
  case DevOps:
  case SecurityEngineer:
    return isSourceCode(p) || isTestFile(p);
  case DatabaseAdmin:
    return isSourceCode(p);
  case UIDesigner:
  case UXDesigner:
    return isDesignAsset(p);
  case FrontendDev:
    return (isSourceCode(p) && !isTestFile(p)) || isDesignAsset(p);
  case BackendDev:
    return isSourceCode(p) && !isTestFile(p);
  case FullStackDev:
    return isSourceCode(p) || isDesignAsset(p);
  case SysAdmin:
    return isSourceCode(p) || isTestFile(p);
  case QA:
    return isTestFile(p);
  case TeamLead:
    return isSourceCode(p) || isTestFile(p) || isDocsOrMgmt(p);
  }
  return false;
}

std::vector<fs::path> View::filter(const std::vector<fs::path> &paths) const {
  std::vector<fs::path> out;
  out.reserve(paths.size());
  for (const auto &p : paths) {
    if (canSee(p)) {
      out.push_back(p);
    }
  }
  return out;
}
