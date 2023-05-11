#include "FormulaParser.hpp"
#include "operands.hpp"

typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;


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
            add("^", BinaryExpression::Pow);
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

        if (f_ok && first_token == f_end && s_ok && second_token == s_end) Equality(f_result, s_result);
        // throw std::runtime_error(std::string("Failed at: `") + first + "`");
        // std::cout << "FormulaParser::parse(exp) : " << exp << "\n";
    }
    else throw std::runtime_error("To much equality");
}

void FormulaParser::Equality(Expression first, Expression second) {

    auto visitor = boost::make_overloaded_function(
        [](double x) -> void { 
            throw std::runtime_error("You cannot assign anything to a number"); 
        },
        [&](const UnaryExpression& first) ->  void {
            throw std::runtime_error("You cannot assign anything to a unary operation");
        },
        [&](const FunctionCall& first) -> void {
            //save function
        },
        [&](const BinaryExpression& first) -> void {
            Equality(first.first, second);
        },
        [&](const VariableExpression &first) -> void {

            auto second_data = eval(second);
            if (second_data.data.size() > 1) 
                throw std::runtime_error("You can't assign multiple values to a constant");
            if (constants.find(first.name) != constants.end()) 
                throw std::runtime_error("You can't override the value of a constant");
            constants[first.name] = second_data.data[0];
        },
        [&](const IndexExpression &first) -> void {
            
            auto second_data = eval(second);
            auto &manager_data = Manager::instance()->GetVariable(QString{first.name.c_str()});

            if (first.args.size() == 1 && first.args[0] < manager_data.measurements.size() && first.args[0] > 0 && second_data.data.size() > 0) {
                manager_data.measurements[first.args[0] - 1] = second_data.data[0];
            } 
            else if (first.args.size() == 1 && (first.args[0] >= manager_data.measurements.size() || first.args[0] <= 0)) {
                throw std::runtime_error("Wrong index");
            } 
            else if (first.args.size() == 2 && first.args[1] < manager_data.measurements.size()) {

                if (manager_data.instrument.type != ErrorType::Calculated) {
                    QList<double> tmp_list;
                    for (int item = 0; item < manager_data.measurements.size(); ++item)
                        tmp_list.append(manager_data.Error(item));
                    
                    manager_data.instrument.type = ErrorType::Calculated;
                    manager_data.instrument.error._type = ErrorValueType::Multiple;
                    manager_data.instrument.error.list.swap(tmp_list);
                }   

                for (int item = first.args[0]; item <= first.args[1]; item++){
                    manager_data.measurements[item - 1] = second_data.data[item - first.args[0]];
                    manager_data.instrument.error.list[item - 1] = second_data.errors[item - first.args[0]];
                }

            }
            else if (first.args.size() == 2 && first.args[1] >= manager_data.measurements.size()) {
                throw std::runtime_error("Wrong index");
            }
            else throw std::runtime_error("Too much indexes");
            return;
        });
    boost::apply_visitor(visitor, first);
}

Node FormulaParser::eval_binary(BinaryExpression::op_t op, Node first, Node second) { 

    switch (op) {
    case BinaryExpression::Plus: return first + second;
    case BinaryExpression::Minus: return first - second;
    case BinaryExpression::Mul: return first * second;
    case BinaryExpression::Div: return first / second;
    case BinaryExpression::Pow: return first ^ second;
    default: throw std::runtime_error("Unknown operator");
    }
}


Node FormulaParser::eval(Expression e) {
    std::vector <double> data{};
    auto visitor = boost::make_overloaded_function(
        [](double x) -> Node {
            return Node(std::vector<double>{x}, std::vector<double>{0}); 
        },
        [&](const UnaryExpression& e) -> Node {
            auto a = eval(e.arg);
            switch (e.op) {
                case UnaryExpression::Plus: return Node(std::vector <double>{+a.data[0]}, a.errors);
                case UnaryExpression::Minus: return Node(std::vector <double>{-a.data[0]}, a.errors);
            }
            throw std::runtime_error("Unknown operator");
        },
        [&](const FunctionCall& e) -> Node {
            
            auto args = eval(e.args[0]).data;
            auto errors = eval(e.args[0]).errors;

            for (int item = 1; item < e.args.size(); item++){
                auto next_args = eval(e.args[item]).data;
                auto next_errors = eval(e.args[item]).errors;
                args.insert(args.end(), next_args.begin(), next_args.end());
                errors.insert(errors.end(), next_errors.begin(), next_errors.end());
            }
            try{
                boost::filesystem::path lib_path(".");
                boost::shared_ptr<plugin_api> plugin = nullptr; 
                std::string plugin_name = "plugin_" + e.function;
                plugin = dll::import_symbol<plugin_api>(lib_path / plugin_name, "plugin", dll::load_mode::append_decorations);
                return {plugin->calculate(Node(args, errors))};
            }
            catch (...){
                throw std::runtime_error("Unknown function");
            }
        },
        [&](const VariableExpression &e) -> Node {
            if (constants.find(e.name) != constants.end()) {
                return Node(std::vector<double>{constants[e.name]});
            }
            throw std::runtime_error("Unknown variable");
        },
        [&](const IndexExpression &e) -> Node {
            try{
                auto manager_data = Manager::instance()->GetRawData(QString{e.name.c_str()});
                if (e.args.size() == 1 && !manager_data.measurements.empty()) {
                    return Node(std::vector<double>{manager_data.measurements[e.args[0] - 1]}, std::vector <double>{manager_data.errors[e.args[0]]}); 
                }
                else if (e.args.size() == 2 && manager_data.measurements.size() >= 2) {
                    if (e.args[1] - e.args[0] < 0 || e.args[1] >= manager_data.measurements.size()) {
                        throw std::runtime_error("Wrong indexes");
                    }
                    std::vector<double> data{manager_data.measurements.begin() + e.args[0] - 1, manager_data.measurements.begin() + e.args[1]};
                    std::vector<double> errors{manager_data.errors.begin() + e.args[0] - 1, manager_data.errors.begin() + e.args[1]};
                    return Node(data, errors);
                }
                else {
                    throw std::runtime_error("Too much indexes");
                }
            }
            catch(std::invalid_argument &error) {
                throw std::runtime_error("No " + e.name + " data");
            }
        },
        [&](const BinaryExpression& e) -> Node {   
            auto first = eval(e.first);
            for (auto&& o : e.ops) {
                auto second = eval(o.second);
                first = eval_binary(o.first, first, second);
            }
            return first;
        });
    return boost::apply_visitor(visitor, e);
}


void FormulaParser::load_constants(std::string path){

    QString name;
    double value = 0;
    QFile csv(QString{"../parser/constants.csv"});

    if (!csv.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("No constant's file");
    QTextStream in(&csv);

    while (!in.atEnd()) {
        in >> name >> value;
        constants[name.toStdString()] = value;
    }
}
