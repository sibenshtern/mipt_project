#include "FormulaParser.hpp"


unary_op_t::unary_op_t() {
    add("+",   UnaryExpression::Plus);
    add("-",   UnaryExpression::Minus);
}


binary_op::binary_op(int precedence) {
    switch (precedence) {
        case 1:
            add("+", BinaryExpression::Plus);
            add("-", BinaryExpression::Minus);
            return;
        case 2:
            add("*",   BinaryExpression::Mul);
            add("/",   BinaryExpression::Div);
            add("mod", BinaryExpression::Mod);
            return;std::to_string(precedence);
        case 3:
            add("**", BinaryExpression::Pow);
            return;
        }
        throw std::runtime_error("Unknown precedence ");
}


Expression FormulaParser::parse(const char* input) {
    auto end = input + std::strlen(input);
    Expression result;
    auto ok = phrase_parse(input, end, expr, x3::space, result);
    if (ok && input == end) return result;
    throw std::runtime_error(std::string("Failed at: `") + input + "`");
}

double FormulaParser::eval_binary(BinaryExpression::op_t op, double a, double b) { 
    switch (op) {
    case BinaryExpression::Plus: return a + b;
    case BinaryExpression::Minus: return a - b;
    case BinaryExpression::Mul: return a * b;
    case BinaryExpression::Div: return a / b;
    case BinaryExpression::Mod: return (int)a % (int)b;
    case BinaryExpression::Pow: return pow(a, b);
    default: throw std::runtime_error("Unknown operator");
    }
}

double FormulaParser::eval(Expression e) {
    auto visitor = boost::make_overloaded_function(
        [](double x) { return x; },
        [&](const UnaryExpression& e) -> double {
            auto a = eval(e.arg);
            switch (e.op) {
            case UnaryExpression::Plus: return +a;
            case UnaryExpression::Minus: return -a;
            }
            throw std::runtime_error("Unknown operator");
        },
        [&](const FunctionCall& e) -> double {
            
            std::vector <double> args;
            for (int item = 0; item < e.args.size(); item++){
                args.push_back(eval(e.args.at(item)));
            }
            //auto args = [&](int i) { return eval(e.args.at(i)); };
            boost::filesystem::path lib_path(".");          // argv[1] contains path to directory with our plugin library
            boost::shared_ptr<plugin_api> plugin = nullptr; 
            std::string plugin_name = "plugin_" + e.function;
            plugin = dll::import_symbol<plugin_api>(lib_path / plugin_name, "plugin", dll::load_mode::append_decorations);
            return plugin->calculate(args);
            throw std::runtime_error("Unknown function");
        },
        [&](const BinaryExpression& e) -> double {   
            auto a = eval(e.first);
            for (auto&& o : e.ops) {
                auto b = eval(o.second);
                a = eval_binary(o.first, a, b);
            }
            return a;
        },
        [](const VariableExpression &e) -> double {
            auto a = e.name;
            std::cout << a;
            return 8;
        });
    return boost::apply_visitor(visitor, e);
}
