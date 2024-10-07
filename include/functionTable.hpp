#ifndef FUNCTION_TABLE_HPP
#define FUNCTION_TABLE_HPP

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class FunctionTable {
public:
  void add(const std::string &name,
           std::function<double(const std::vector<double> &)> func);

  auto get(const std::string &name) const
      -> std::function<double(const std::vector<double> &)>;

private:
  std::unordered_map<std::string,
                     std::function<double(const std::vector<double> &)>>
      functions_;
};

#endif // !FUNCTION_TABLE_HPP
