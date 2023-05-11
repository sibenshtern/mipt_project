#include <boost/config.hpp>
#include "plugin_api.hpp"
#include <iostream>


namespace plugin_namespace {

    class plugin_arithmetic_average: public plugin_api {
    public:
        plugin_arithmetic_average() {}

        Node calculate(Node input_node) {
            double result = 0;
            double error = 0;
            for (int item = 0; item < input_node.data.size(); ++item){
                result += input_node.data[item];
                error += input_node.errors[item];
            }
            if (input_node.data.size() > 0) {
                return Node(std::vector<double> {result / input_node.data.size()}, std::vector <double> {error});
            }
            else {
                throw std::runtime_error("No input in function");
            }

        }

        ~plugin_arithmetic_average() {}
    };

    extern "C" BOOST_SYMBOL_EXPORT plugin_arithmetic_average plugin;
    plugin_arithmetic_average plugin;

}