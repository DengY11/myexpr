#include "../include/functionTable.hpp"
#include <cmath>
#include <stdexcept>
#include <unordered_map>
namespace myexpr {

FunctionTable::FunctionTable() {
  std::unordered_map<std::string,
                     std::function<double(const std::vector<double> &)>>
      initial_funcs{
          {"cos",
           [](std::vector<double> arg) -> double { return std::cos(arg[0]); }},
          {"sin",
           [](std::vector<double> arg) -> double { return std::sin(arg[0]); }},
          {"tan",
           [](std::vector<double> arg) -> double { return std::tan(arg[0]); }},
          {"log",
           [](std::vector<double> arg) -> double { return std::log(arg[0]); }},
          {"sqrt",
           [](std::vector<double> arg) -> double { return std::sqrt(arg[0]); }},
          {"abs",
           [](std::vector<double> arg) -> double { return std::abs(arg[0]); }}

      };
  functions_ = std::move(initial_funcs);
}

FunctionTable::FunctionTable(
    std::unordered_map<std::string,
                       std::function<double(const std::vector<double> &)>>
        &adding_funcs)
    : FunctionTable() {
  for (auto [name, func] : adding_funcs) {
    this->functions_.insert({name, func});
  }
}

void FunctionTable::add(
    const std::string &name,
    std::function<double(const std::vector<double> &)> func) {
  if (this->functions_.find(name) != this->functions_.end()) {
    throw std::runtime_error("function has already exsit: " + name);
  }
  functions_.insert({name, func});
}

auto FunctionTable::get(const std::string &name) const
    -> std::function<double(const std::vector<double> &)> {
  if (this->functions_.find(name) == this->functions_.end()) {
    throw std::runtime_error("function: " + name + " not exsit");
  }
  return functions_.at(name);
}

auto FunctionTable::exist(const std::string &name) -> bool {
  return this->functions_.find(name) != this->functions_.end();
}

} // namespace myexpr
