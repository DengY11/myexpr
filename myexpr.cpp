#include "myexpr.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <climits>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>
#include <string>
#include <iterator>

//constant value
#ifndef NAN
#define NAN (std::numeric_limits<double>::quiet_NaN())
#endif

#ifndef INFINITY
#define INFINITY (std::numeric_limits<double>::infinity())
#endif

//some helper function

static double pi() { return 3.14159265358979323846; }
static double e() { return 2.71828182845904523536; }
static double add(double a, double b) { return a + b; }
static double sub(double a, double b) { return a - b; }
static double mul(double a, double b) { return a * b; }
static double divide(double a, double b) {return a / b; }
static double negate(double a) {return -a; }
static double comma(double a, double b) { (void)a; return b; }

static double fac(double a) {
    if (a < 0.0) return NAN;
    if (a > UINT_MAX) return INFINITY;
    unsigned int ua = static_cast<unsigned int>(a);
    unsigned long int result = 1, i;
    for (i = 1; i <=ua; ++i){
        if (i > ULLONG_MAX / result) return INFINITY;
        result *= i;
    }
    return (double)result;
}

static double ncr(double n, double r){
    if (n < 0.0 || r < 0.0 || n < r) return NAN;
    if (n > UINT_MAX || r> UINT_MAX) return INFINITY;
    unsigned long int un = static_cast<unsigned int>(n), ur = static_cast<unsigned int>(r), i;
    unsigned long int result = 1;
    if (ur > un / 2) ur = un - ur;
    for(i = 1; i <= ur; ++i){
        if (result > ULONG_MAX / (un - ur +i)) return INFINITY;
        result *= un - ur + i;
        result /= i;
    }
    return static_cast<double>(result);
}

static double npr(double n, double r) { return ncr(n,r) * fac(r); }

struct State {
    std::string start; // the string of expr
    std::string::const_iterator next; // now position of expr
    int type;
    double value;
    const double* bound;
    std::function<double()> function;
    std::function<double(const std::vector<double>&)> function_with_args;
    void* context;
    std::string current_func_name;
    std::vector<TeVariable> lookup; // table of variables
};

// an vector called "function", element type is TeVariable
static const std::vector<TeVariable> functions = {
         {"abs", [](const std::vector<double>& args) -> double {return std::fabs(args[0]); },TE_FUNCTION | TE_FLAG_PURE,nullptr},
         {"acos", [](const std::vector<double>&args) -> double {return std::acos(args[0]);},TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"asin", [](const std::vector<double>& args) -> double {return std::asin(args[0]);},TE_FUNCTION | TE_FLAG_PURE,nullptr},
         {"atan", [](const std::vector<double>& args) -> double {return std::atan(args[0]);},TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"atan2", [](const std::vector<double>& args) -> double {return std::atan2(args[0], args[1]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"ceil", [](const std::vector<double>& args) -> double {return std::ceil(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"cos", [](const std::vector<double>& args) -> double {return std::cos(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"cosh", [](const std::vector<double>& args) -> double {return std::cosh(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"e",[](const std::vector<double>& _) -> double {return 2.718281828459045;}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"exp", [](const std::vector<double>& args) -> double {return std::exp(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"fac", [](const std::vector<double>& args) -> double {return fac(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"floor", [](const std::vector<double>& args) -> double {return std::floor(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"ln", [](const std::vector<double>& args) -> double {return std::log(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
#ifdef TE_NAT_LOG
        {"log",[](const std::vector<double>& args) -> double {return std::log(x);},TE_FUNCTION | TE_FLAG_PURE, nullptr},
#else
         {"log",[](const std::vector<double>& args) -> double {return std::log10(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
#endif
         {"log10",[](const std::vector<double>& args) -> double {return std::log10(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"ncr", [](const std::vector<double>& args) -> double {return ncr(args[0], args[1]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"npr", [](const std::vector<double>& args) -> double {return npr(args[0], args[1]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"pi",[](const std::vector<double>& _) -> double {return 3.141592653589793;}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"pow", [](const std::vector<double>& args) -> double {return std::pow(args[0], args[1]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"sin", [](const std::vector<double>& args) -> double {return std::sin(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"sinh", [](const std::vector<double>& args) -> double {return std::sinh(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"sqrt", [](const std::vector<double>& args) -> double {return std::sqrt(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"tan", [](const std::vector<double>& args) -> double {return std::tan(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {"tanh", [](const std::vector<double>& args) -> double {return std::tanh(args[0]);}, TE_FUNCTION | TE_FLAG_PURE, nullptr},
         {nullptr, nullptr, 0 ,nullptr}
};

static const TeVariable* find_builtin(const std::string& name){
    auto it = std::find_if(functions.begin(), functions.end(), [&name](const TeVariable& var){
        return var.name == name;
    });
    return (it != functions.end()) ? &(*it) : nullptr;
}

static const TeVariable* find_lookup(const State& s, const std::string& name){
    auto it = std::find_if(s.lookup.begin(), s.lookup.end(), [&name](const TeVariable& var){
        return var.name == name;
    });
    return (it != s.lookup.end()) ? &(*it) : nullptr;
}

static void next_token(State& s) {
    s.type = 0;
    s.current_func_name.clear();

    while (s.next != s.start.end()) {
        if (std::isspace(*s.next)) {
            ++s.next;
            continue;
        }

        // try to get the first number
        if (std::isdigit(*s.next) || *s.next == '.') {
            const char* start_ptr = &(*s.next);
            char* end_ptr;
            s.value = std::strtod(start_ptr, &end_ptr);
            s.next += (end_ptr - start_ptr);
            s.type = 1;
            return;
        }

        if (std::isalpha(*s.next)) {
            auto start = s.next;
            while (s.next != s.start.end() && (std::isalpha(*s.next) || std::isdigit(*s.next) || *s.next == '_')) {
                ++s.next;
            }

            std::string name(start, s.next);
            const TeVariable* var = find_lookup(s, name);
            if (!var) var = find_builtin(name);

            if (var) {
                s.type = var->type;
                s.function_with_args = var->function_with_args;
                s.current_func_name = var->name;
            } else {
                s.type = -2;
            }
            return;
        }

        switch (*s.next) {
            case '+':
                s.type = 2;
                s.function_with_args = [](const std::vector<double>& args) { return add(args[0], args[1]); };
                s.current_func_name = "+";
                break;
            case '-':
                s.type = 2;
                s.function_with_args = [](const std::vector<double>& args) { return sub(args[0], args[1]); };
                s.current_func_name = "-";
                break;
            case '*':
                s.type = 2;
                s.function_with_args = [](const std::vector<double>& args) { return mul(args[0], args[1]); };
                s.current_func_name = "*";
                break;
            case '/':
                s.type = 2;
                s.function_with_args = [](const std::vector<double>& args) { return divide(args[0], args[1]); };
                s.current_func_name = "/";
                break;
            case '^':
                s.type = 2;
                s.function_with_args = [](const std::vector<double>& args) { return std::pow(args[0], args[1]); };
                s.current_func_name = "^";
                break;
            case '%':
                s.type = 2;
                s.function_with_args = [](const std::vector<double>& args) { return std::fmod(args[0], args[1]); };
                s.current_func_name = "%";
                break;
            case '(':
                s.type = 3;
                break;
            case ')':
                s.type = 4;
                break;
            case ',':
                s.type = 5;
                break;
            default:
                s.type = -2;
                break;
        }

        if (s.type != 0) {
            ++s.next;
            return;
        }
    }
    s.type = -1;
}


std::unique_ptr<TeExpr> base(State& s);
std::unique_ptr<TeExpr> power(State& s);

std::unique_ptr<TeExpr> list(State& s);
std::unique_ptr<TeExpr> expr(State& s);



std::unique_ptr<TeExpr> base(State& s) {
    std::unique_ptr<TeExpr> ret;
    int arity;

    switch (s.type) {
        case 1:
            ret = std::make_unique<TeExpr>(s.value);
            next_token(s);
            break;

        case 0:
            ret = std::make_unique<TeExpr>(s.bound);
            next_token(s);
            break;

        case 3:
            next_token(s);
            ret = list(s);
            if ( s.type != 4){
                s.type = -2;
            }
            else{
                next_token(s);
            }
            break;

        default:
            ret = std::make_unique<TeExpr>();
            s.type = -2;
            ret->value = NAN;
            break;
    }
    return ret;
}

std::unique_ptr<TeExpr> power(State& s){
    int sign = 1;
    while (s.type == 2 && (s.next != s.start.end() && (*s.next == '-'))) {
        sign = -sign;
        next_token(s);
    }

    std::unique_ptr<TeExpr> ret = base(s);

    if (sign == -1) {
        auto temp = std::make_unique<TeExpr>([value = ret->value]() ->double {
            return  -value;
        });
        ret = std::move(temp);
    }
    return ret;
}

std::unique_ptr<TeExpr> factor(State &s){
    std::unique_ptr<TeExpr> ret = power(s);
    while (s.type == 2 && s.current_func_name == "pow") {
        auto t = s.function_with_args;
        next_token(s);
        std::unique_ptr<TeExpr> p = power(s);

        TeExpr* prev_ptr = ret.release();
        TeExpr* p_ptr = p.release();

        ret = std::make_unique<TeExpr>([prev_ptr, p_ptr, t]() -> double {
            std::vector<double> args = {prev_ptr->value, p_ptr->value};
            double result = t(args);
            delete prev_ptr;
            delete p_ptr;
            return result;
        });
    }
    return ret;
}

std::unique_ptr<TeExpr> term(State& s){
    std::unique_ptr<TeExpr> ret = factor(s);
    while(s.type == 2 && (s.current_func_name == "*" ||
                          s.current_func_name == "/" ||
                          s.current_func_name == "%")){
        auto t = s.function_with_args;
        next_token(s);
        std::unique_ptr<TeExpr> f = factor(s);

        TeExpr* prev_ptr = ret.release();
        TeExpr* f_ptr = f.release();

        ret = std::make_unique<TeExpr>([prev_ptr, f_ptr, t]() ->double {
            std::vector<double>args = {prev_ptr->value, f_ptr->value};
            double result = t(args);
            delete prev_ptr;
            delete f_ptr;
            return result;
        });
    }
    return ret;
}

std::unique_ptr<TeExpr> expr(State& s) {
    std::unique_ptr<TeExpr> ret = term(s);

    while(s.type == 2 && (s.current_func_name == "+" ||
                          s.current_func_name == "-")){
        auto t = s.function_with_args;
        next_token(s);
        std::unique_ptr<TeExpr> te = term(s);

        TeExpr* prev_ptr = ret.release();
        TeExpr* te_ptr = te.release();

        ret = std::make_unique<TeExpr>([prev_ptr, te_ptr, t]() ->double{
            std::vector<double> args = {prev_ptr->value, te_ptr->value};
            double result = t(args);
            delete prev_ptr;
            delete te_ptr;
            return result;
        });
    }
    return ret;
}

std::unique_ptr<TeExpr> list(State& s) {
    std::unique_ptr<TeExpr> ret = expr(s);

    while(s.type == 5){
        next_token(s);
        std::unique_ptr<TeExpr> e = expr(s);

        TeExpr* prev_ptr = ret.release();
        TeExpr* e_ptr = e.release();

        ret = std::make_unique<TeExpr>([prev_ptr, e_ptr]() -> double {
            double result = e_ptr->value;
            delete prev_ptr;
            delete e_ptr;
            return result;
        });
    }
    return ret;
}

void te_print(const TeExpr* n, int depth = 0){
    for (int i = 0; i<depth; ++i) std::cout << ' ';
    switch (n->type) {
        case TeExprType::VALUE:
            std::cout << n->value << std::endl;
            break;
        case TeExprType::BOUND:
            std::cout << "bound" << std::endl;
            break;
        case TeExprType::FUNCTION:
        case TeExprType::FUNCTION_WITH_ARGS:
            std::cout << "f" << n->parameters.size() << std::endl;
            for (const auto& param : n->parameters) {
                te_print(param.get(), depth + 1);
            }
            break;
    }
}

double te_eval(const TeExpr* n) {
    if (!n) return NAN;

    switch (n->type) {
        case TeExprType::VALUE:
            return n->value;
        case TeExprType::BOUND:
            return *n->bound;
        case TeExprType::FUNCTION:
            return n->function();
        case TeExprType::FUNCTION_WITH_ARGS: {
            std::vector<double> args;
            for (const auto& param : n->parameters) {
                args.push_back(te_eval(param.get()));
            }
            return n->function_with_args(args);
        }
        default:
            return NAN;
    }
}


void te_free(std::unique_ptr<TeExpr> &n) {
    n.reset();
}

std::unique_ptr<TeExpr> te_compile(const std::string& expression, const std::vector<TeVariable>& variables, int* error){
    State s;
    s.start = expression;
    s.next = s.start.begin();
    s.lookup = variables;

    next_token(s);
    std::unique_ptr<TeExpr> root = list(s);
    if (root == nullptr) {
        if (error) *error = -1;
        return nullptr;
    }

    if (s.type != -1) {
        if (error) {
            *error = static_cast<int>(s.next - s.start.begin());
            if (*error == 0) *error = 1;
        }
        return nullptr;
    } else {
        if (error) *error = 0;
        return root;
    }
}

double te_interp(const std::string& expression, int* error) {
    std::vector<TeVariable> empty_variables;
    std::unique_ptr<TeExpr> n = te_compile(expression, empty_variables, error);

    if (n) {
        double ret = te_eval(n.get());
        te_free(n);
        return ret;
    } else {
        return NAN;
    }
}























