#include "../include/expr_calculator.hpp"

namespace myexpr {

expr_calculator::expr_calculator(std::string &expression)
    : paser_(expression) {}

expr_calculator::expr_calculator(std::string &expression,
                                 FunctionMap &function_map)
    : paser_(expression) {
  this->paser_.add_function_map(function_map);
}

expr_calculator::expr_calculator(std::string &expression,
                                 VariableMap &variable_map)
    : paser_(expression) {
  this->paser_.add_variable_map(variable_map);
}

expr_calculator::expr_calculator(std::string &expression,
                                 VariableMap &variable_map,
                                 FunctionMap &function_map)
    : paser_(expression) {
  this->paser_.add_function_map(function_map);
  this->paser_.add_variable_map(variable_map);
}

expr_calculator::expr_calculator(std::string &expression,
                                 FunctionMap &function_map,
                                 VariableMap &variable_map)
    : myexpr::expr_calculator(expression, variable_map, function_map) {}

} // namespace myexpr
