#ifndef MYEXPR_HPP
#define MYEXPR_HPP

#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <cmath>
#include <unordered_map>
#include <memory>

enum class TeExprType{
    VALUE,
    BOUND,
    FUNCTION,
    FUNCTION_WITH_ARGS
};


// the Class of expressions
class TeExpr {
public:
    TeExprType type;
    double value;
    const double* bound;
    std::function<double()> function;
    std::function<double(const std::vector<double>&)> function_with_args;
    std::vector<std::unique_ptr<TeExpr>> parameters;

    TeExpr() : type(TeExprType::VALUE), value(0), bound(nullptr), function(nullptr), function_with_args(nullptr) {}
    explicit TeExpr(double val) : type(TeExprType::VALUE), value(val), bound(nullptr), function(nullptr), function_with_args(nullptr) {}
    explicit TeExpr(const double* bnd) : type(TeExprType::BOUND), value(0), bound(bnd), function(nullptr), function_with_args(nullptr) {}
    explicit TeExpr(std::function<double()>func) : type(TeExprType::FUNCTION), value(0), bound(nullptr),
        function(std::move(func)), function_with_args(nullptr){}
    explicit TeExpr(std::function<double(const std::vector<double>&)> func) :type(TeExprType::FUNCTION_WITH_ARGS),value(0), bound(nullptr),
        function(nullptr), function_with_args(std::move(func)) {}

    template<typename F>
    explicit TeExpr(F&& func) : type(TeExprType::FUNCTION), value(0), bound(nullptr), function(std::forward<F>(func)), function_with_args(
                nullptr){}
};

enum {
    TE_VARIABLE = 0,
    TE_FUNCTION = 1,
    TE_FLAG_PURE = 32
};

struct TeVariable{
    std::string name;
    std::function<double(const std::vector<double>&)> function_with_args;
    int type;
    void* context;
};


double te_interp(const std::string& expression, int* error);
std::unique_ptr<TeExpr> te_compile(const std::string& expression, const std::vector<TeExprType>& variables, int *error);
double te_eval(const TeExpr* n);
void te_print(const TeExpr*n);
void te_free(std::unique_ptr<TeExpr>& n);



#endif