#include "../include/evaluator.hpp"
#include <cassert>
#include <cctype>
#include <cmath>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
namespace myexpr {
Evaluator::Evaluator(std::shared_ptr<ExprNode> root) : root_(root) {}

} // namespace myexpr
