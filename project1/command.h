#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <vector>
#include <string>

enum CommandName { ORIGIN, FORWARD, RIGHT, LEFT, PEN_DOWN, PEN_UP, COLOR };

class Command {
 public:
  Command(CommandName name, double arg = 0) : _name(name), _arg(arg) {}
  CommandName name() const { return _name; }
  double arg() const { return _arg; }
 private:
  CommandName _name;
  double _arg;
};

std::vector<Command> Parse(const std::string& filename);

#endif
