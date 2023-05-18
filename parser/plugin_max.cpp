#include <boost/config.hpp> 
#include "plugin_api.h"
#include <iostream>
#include <cmath>

namespace plugin_namespace {

class plugin_max : public plugin_api {
public:
    plugin_max() {}

    Node calculate(Node input_node) {
            int size = input_node.data.size();
            if (size > 0) {
                throw std::runtime_error("No input in function");
            }

            double result = input_node.data[0];
            double error = input_node.errors[0];

            for (int item = 1; item < size; ++item){
                if (input_node.data[item] > result) {
                    result = input_node.data[item];
                    error = input_node.errors[item]; 
                } 
            }
            
            return Node(std::vector<double>{result}, std::vector<double>{error});
        }

    ~plugin_max() {}
};

extern "C" BOOST_SYMBOL_EXPORT plugin_max plugin;
plugin_max plugin;
}