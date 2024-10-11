#include "../include/functionTable.hpp"
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <unordered_map>
namespace myexpr {

FunctionTable::FunctionTable() {
  std::unordered_map<std::string,
                     std::function<double(const std::vector<double> &)>>
      initial_funcs{
          {"cos",
           [](std::vector<double> arg) -> double {
             if (arg.size() != 1) {
               throw std::runtime_error(
                   "function cos needs only one argument!");
             }
             return std::cos(arg[0]);
           }},
          {"sin",
           [](std::vector<double> arg) -> double {
             if (arg.size() != 1) {
               throw std::runtime_error(
                   "function sin needs only one argument!");
             }
             return std::sin(arg[0]);
           }},
          {"tan",
           [](std::vector<double> arg) -> double {
             if (arg.size() != 1) {
               throw std::runtime_error(
                   "function tan needs only one argument!");
             }
             return std::tan(arg[0]);
           }},
          {"log",
           [](std::vector<double> arg) -> double {
             if (arg.size() != 1) {
               throw std::runtime_error(
                   "function arg needs only one argument!");
             }
             return std::log(arg[0]);
           }},
          {"sqrt",
           [](std::vector<double> arg) -> double {
             if (arg.size() != 1) {
               throw std::runtime_error(
                   "function sqrt needs only one argument!");
             }
             return std::sqrt(arg[0]);
           }},
          {"abs",
           [](std::vector<double> arg) -> double {
             if (arg.size() != 1) {
               throw std::runtime_error(
                   "function abs needs only one argument!");
             }
             return std::abs(arg[0]);
           }},
          {"pow",
           [](std::vector<double> args) -> double {
             if (args.size() != 2) {
               throw std::runtime_error("function pow needs two arguments!");
             }
             return std::pow(args[0], args[1]);
           }},
          {"averge",
           [](std::vector<double> args) -> double {
             if (args.empty()) {
               throw std::runtime_error(
                   "function averge needs at least one argument!");
             }
             double ret = 0.0;
             size_t cnt = 0;
             for (const auto &number : args) {
               ++cnt;
               ret += (static_cast<double>(number) - ret) / cnt;
             }
             return ret;
           }}

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
