#include "../include/expr_calculator.hpp"
#include <stdexcept>

namespace myexpr {

expr_calculator::expr_calculator(std::string &expression)
    : paser_(expression), inited_(true) {}

expr_calculator::expr_calculator(std::string &expression,
                                 FunctionMap &function_map)
    : paser_(expression), inited_(true) {
  this->paser_.add_function_map(function_map);
}

expr_calculator::expr_calculator(std::string &expression,
                                 VariableMap &variable_map)
    : paser_(expression), inited_(true) {
  this->paser_.add_variable_map(variable_map);
}

expr_calculator::expr_calculator(std::string &expression,
                                 VariableMap &variable_map,
                                 FunctionMap &function_map)
    : paser_(expression), inited_(true) {
  this->paser_.add_function_map(function_map);
  this->paser_.add_variable_map(variable_map);
}

expr_calculator::expr_calculator(std::string &expression,
                                 FunctionMap &function_map,
                                 VariableMap &variable_map)
    : myexpr::expr_calculator(expression, variable_map, function_map) {}

void expr_calculator::calculate() {
  if (!inited_) {
    throw std::runtime_error("calculator needs an expression");
  }

  auto root = this->paser_.parse();
  myexpr::Evaluator evaluator(root);

  this->result_ = evaluator.evaluate(this->paser_.get_variable_table(),
                                     this->paser_.get_function_table());

} // TODO::
auto expr_calculator::get_result() -> double { return this->result_; } // TODO::

} // namespace myexpr
