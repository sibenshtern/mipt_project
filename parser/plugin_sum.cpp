#include <boost/config.hpp> 
#include "plugin_api.h"
#include <iostream>
#include <cmath>

namespace plugin_namespace {

class plugin_sum : public plugin_api {
public:
    plugin_sum() {}

    Node calculate(Node input_node) {
            double result = 0;
            double error = 0;
            int size = input_node.data.size();
            for (int item = 0; item < size; ++item){
                result += input_node.data[item];
                error += std::pow(input_node.errors[item], 2);
            }
            if (input_node.data.size() > 0) {
                Node node = Node(std::vector<double>{result}, std::vector<double>{error});
                return node;
            }
            else throw std::runtime_error("No input in function");
        }

    ~plugin_sum() {}
};

extern "C" BOOST_SYMBOL_EXPORT plugin_sum plugin;
plugin_sum plugin;
}