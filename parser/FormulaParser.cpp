#include "FormulaParser.hpp"
#include "operands.hpp"
#include "../logic/manager.h"

typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
equality_op_t::equality_op_t(){
    add ("=", EqualityExpression::Equality);
}

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
            return;
        case 3:
            add("**", BinaryExpression::Pow);
            return;
        throw std::runtime_error("Unknown precedence ");
    }
}

void FormulaParser::parse(std::string input_t){

    input_t.erase(std::remove_if(input_t.begin(), input_t.end(), isspace), input_t.end());

    if(input_t.find("=") == 0) throw std::runtime_error("Тo data to the right of equality");
    else if(input_t.find("=") >= input_t.size()) throw std::runtime_error("No equality");
    else if(input_t.find("=") == 0) throw std::runtime_error("Тo data to the right of equality");
    else if(input_t.find("=") == input_t.rfind("=")){
        std::string first_token_t = input_t.substr(0, input_t.find("="));
        std::string second_token_t = input_t.substr(input_t.find("=")+1, input_t.size() - 1);

        const char * first_token = first_token_t.c_str();
        const char * second_token = second_token_t.c_str();

        auto f_end = first_token + std::strlen(first_token);
        auto s_end = second_token + std::strlen(second_token);

        Expression f_result;
        Expression s_result;
        auto f_ok = phrase_parse(first_token, f_end, expr, x3::space, f_result);
        auto s_ok = phrase_parse(second_token, s_end, expr, x3::space, s_result);

        Node firstNode = Node(f_result);
        Node secondNode = Node(s_result);

        if (f_ok && first_token == f_end && s_ok && second_token == s_end) Equality(f_result, s_result);
        // throw std::runtime_error(std::string("Failed at: `") + first + "`");
        // std::cout << "FormulaParser::parse(exp) : " << exp << "\n";
    }
    else throw std::runtime_error("To much equality");
}

void FormulaParser::Equality(Expression first, Expression second) {
    auto visitor = boost::make_overloaded_function(
        [](double x) -> void { throw std::runtime_error("You cannot assign anything to a number"); },

        [&](const UnaryExpression& first) ->  void {
            throw std::runtime_error("You cannot assign anything to a unary operation");
        },
        [&](const FunctionCall& first) -> void {
           return;
        },
        [&](const BinaryExpression& first) -> void {
            Equality(first.first, second);
        },
        [&](const VariableExpression &first) -> void {
            auto data = eval(second);
            if (data.size() > 1) throw std::runtime_error("You can't assign multiple values to a constant");
            constants[first.name] = data[0];
            return;
        },
        [&](const IndexExpression &first) -> void {
            auto data = eval(second);
            std::cout << data;
            auto &manager_data = Manager::instance()->GetVariable(QString{first.name.c_str()});
            std::cout << first.args.size() << " " <<  data.size();
            qInfo() << manager_data.measurements;
            if (first.args.size() == 1 && data.size() == 1){
                manager_data.measurements[first.args[0]] = data[0];
            }  // manager.return_variable(e.name, e.args.at(0));
            else if (first.args.size() == 2 && first.args[1] - first.args[0] + 1 == data.size()) {
                for (int item = first.args[0]; item <= first.args[1]; item++)
                    manager_data.measurements[item] = data[item];
            }
            else throw std::runtime_error("Too much indexes");
            return;
        });
    boost::apply_visitor(visitor, first);
}

std::vector<double> FormulaParser::eval_binary(BinaryExpression::op_t op, std::vector<double> a, std::vector<double> b) { 

    switch (op) {
    case BinaryExpression::Plus: return a + b;
    case BinaryExpression::Minus: return a - b;
    case BinaryExpression::Mul: return a * b;
    case BinaryExpression::Div: return a / b;
    case BinaryExpression::Pow: return a ^ b;
    default: throw std::runtime_error("Unknown operator");
    }
}
//std::vector<double> errors
std::vector<double> FormulaParser::eval(Expression e) {
    std::vector <double> data{};
    auto visitor = boost::make_overloaded_function(
        [](double x) -> std::vector<double> {
            return std::vector<double>{x}; 
        },
        [&](const UnaryExpression& e) -> std::vector <double> {
            auto a = eval(e.arg)[0];
            switch (e.op) {
                case UnaryExpression::Plus: return std::vector<double>{+a};
                case UnaryExpression::Minus: return std::vector<double>{-a};
            }
            throw std::runtime_error("Unknown operator");
        },
        [&](const FunctionCall& e) -> std::vector<double> {
            std::vector <double> args = eval(e.args[0]);
            for (int item = 1; item < e.args.size(); item++){
                auto c = eval(e.args[item]);
                args.insert(args.end(), c.begin(), c.end());
            }
            try{
                boost::filesystem::path lib_path(".");
                boost::shared_ptr<plugin_api> plugin = nullptr; 
                std::string plugin_name = "plugin_" + e.function;
                plugin = dll::import_symbol<plugin_api>(lib_path / plugin_name, "plugin", dll::load_mode::append_decorations);
                return std::vector<double>{plugin->calculate(args)};
            }
            catch (...){
                throw std::runtime_error("Unknown function");
            }
        },
        [&](const VariableExpression &e) -> std::vector<double> {
            auto a = e.name;
            if (constants.find(a) != constants.end()) return std::vector<double>{constants[a]};
            throw std::runtime_error("Unknown variable");
        },
        [&](const IndexExpression &e) -> std::vector<double> {
            auto manager_data = Manager::instance()->GetRawData(QString{e.name.c_str()});

            if (e.args.size() == 1 && !manager_data.measurements.empty()) return std::vector<double>{manager_data.measurements[e.args[0]]}; // manager.return_variable(e.name, e.args.at(0));
            else if (e.args.size() == 2 && manager_data.measurements.size() >= 2) {
                qInfo() << e.args;
                // if (e.args[1] - e.args[0] < 0) throw std::runtime_error("Wrong indexes");
                for (int item = e.args[0]; item <= e.args[1]; item++) {
                        data.push_back(manager_data.measurements[item]);
                    }
                    return data;
            }
            throw std::runtime_error("Too much indexes1");
        },
        [&](const BinaryExpression& e) -> std::vector<double> {   
            auto a = eval(e.first);
            for (auto&& o : e.ops) {
                auto b = eval(o.second);
                a = eval_binary(o.first, a, b);
            }
            return a;
        });
    return boost::apply_visitor(visitor, e);
}

void FormulaParser::load_constants(std::string path){
    std::string name;
    double value = 0;
    std::fstream fin;
    try{
        fin.open(path + "constants.csv", std::ios::in);
        while(fin){
            fin >> name >> value;
            constants[name] = value;
        }
        fin.close();
    }
    catch (...){
        throw std::runtime_error("No constant's file");
    }
}
