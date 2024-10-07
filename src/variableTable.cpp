#include "variableTable.hpp"
#include <stdexcept>

namespace myexpr {

void VariableTable::set(const std::string &name, double value) {
  this->variables_[name] = value;
}

auto VariableTable::get(const std::string &name) const -> double {
  auto it = this->variables_.find(name);
  if (it != this->variables_.end()) {
    return it->second;
  }
  throw std::runtime_error("Variable not found: " + name);
}

} // namespace myexpr
