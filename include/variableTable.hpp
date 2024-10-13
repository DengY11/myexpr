#ifndef VARIABLE_TABLE_HPP
#define VARIABLE_TABLE_HPP

#include <string>
#include <unordered_map>
namespace myexpr {

class VariableTable {
public:
  VariableTable() = default;
  VariableTable(std::unordered_map<std::string, double> &initial_var);
  void set(const std::string &name, double value);
  auto get(const std::string &name) const -> double;
  auto exit(const std::string &name) const -> bool;

private:
  std::unordered_map<std::string, double> variables_;
};

} // namespace myexpr
#endif // !VARIABLE_TABLE_HPP
