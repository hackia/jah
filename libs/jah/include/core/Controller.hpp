#pragma once
#include "../config/Config.hpp"
#include "Model.hpp"
#include "View.hpp"

namespace Jah {
class Controller {
public:
  explicit Controller(const Config &config, const Observer &observer);
  static string ask(const string &question, const vector<string> &samples = {});
  static int initialize();
  static int commit(const Config & config);
  int run(int argc, const char **argv) const;
  static int help(const char **argv);

private:
  Config config;
  Model model;
  View view;
  Observer observer;
};
} // namespace Jah