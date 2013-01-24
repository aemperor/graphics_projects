#include <vector>
#include <string>
#include <fstream>

#include "./command.h"

std::vector<Command> Parse(std::istream* pin) {
  std::vector<Command> commands;
  std::istream& in = *pin;
  while (in) {
    std::string s;
    double d;
    in >> s;
    if (!s.empty()) {
      CommandName name;
      if (s == "left") {
        name = LEFT;
        in >> d;
      } else if (s == "right") {
        name = RIGHT;
        in >> d;
      } else if (s == "forward") {
        name = FORWARD;
        in >> d;
      } else if (s == "up") {
        name = PEN_UP;
      } else if (s == "down") {
        name = PEN_DOWN;
      } else if (s == "color") {
        name = COLOR;
        in >> d;
      } else if (s == "origin") {
        name = ORIGIN;
      } else if (s == "loop") {
        int loopIters;
        in >> loopIters;
        in >> s;
        std::vector<Command> loop = Parse(pin);
        for (int i = 0; i < loopIters; ++i) {
          commands.insert(commands.end(), loop.begin(), loop.end());
        }
        s = "";
      } else if (s == "}") {
        return commands;
        s = "";
      }

      if (!s.empty()) {
        commands.push_back(Command(name, d));
      }
    }
  }
  return commands;
}

std::vector<Command> Parse(const std::string& filename) {
  std::ifstream in(filename.c_str());
  return Parse(&in);
}
