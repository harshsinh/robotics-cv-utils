#pragma once

#include <map>
#include <string>
#include <vector>


class ArgParser {
 public:
  ArgParser (int &argc, char **argv, std::map<std::string, std::string> args = None) {}
}
