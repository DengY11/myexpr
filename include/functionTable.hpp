#ifndef FUNCTION_TABLE_HPP
#define FUNCTION_TABLE_HPP

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
namespace myexpr {
class FunctionTable {
public:
  FunctionTable();
  FunctionTable(
      std::unordered_map<std::string,
                         std::function<double(const std::vector<double> &)>> &);
  void add(const std::string &name,
           std::function<double(const std::vector<double> &)> func);

  auto get(const std::string &name) const
      -> std::function<double(const std::vector<double> &)>;

  auto exist(const std::string &name) -> bool;

private:
  std::unordered_map<std::string,
                     std::function<double(const std::vector<double> &)>>
      functions_;
};
} // namespace myexpr
#endif // !FUNCTION_TABLE_HPP
