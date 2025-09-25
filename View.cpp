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
  if (nameContains(p, "test") || nameContains(p, "spec") ||
      nameContains(p, "fixture"))
    return true;
  if (isInDirNamed(p, "test") || isInDirNamed(p, "tests") ||
      isInDirNamed(p, "testing"))
    return true;
  static const char *patterns[] = {"test.cpp", "_test.cpp", ".spec.", ".test.",
                                   ".feature"};
  const std::string fnLower = toLower(p.filename().string());
  return std::any_of(std::begin(patterns), std::end(patterns),
                     [&fnLower](const char *pat) {
                       return fnLower.find(pat) != std::string::npos;
                     });
}

bool isDesignAsset(const fs::path &p) {
  static const char *extensions[] = {
      "css",  "scss", "sass", "less", "styl", "svg", "png",  "jpg",
      "jpeg", "gif",  "webp", "ico",  "ttf",  "otf", "woff", "woff2"};
  const std::string e = extOf(p);
  if (std::any_of(std::begin(extensions), std::end(extensions),
                  [&e](const char *ex) { return e == ex; }))
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
  return std::any_of(std::begin(extensions), std::end(extensions),
                     [&e](const char *extension) { return e == extension; });
}

bool isDocsOrMgmt(const fs::path &p) {
  static const char *docExtensions[] = {"md", "txt", "rst", "pdf", "csv"};
  const std::string e = extOf(p);
  if (std::any_of(std::begin(docExtensions), std::end(docExtensions),
                  [&e](const char *ex) { return e == ex; }))
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
  out.reserve(paths.size());
  std::copy_if(paths.begin(), paths.end(), std::back_inserter(out),
               [this](const fs::path &p) { return canSee(p); });
  return out;
}