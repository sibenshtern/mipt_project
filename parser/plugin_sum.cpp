#include <boost/config.hpp> 
#include "plugin_api.hpp"
#include <iostream>

namespace plugin_namespace {

class plugin_sum : public plugin_api {
public:
    plugin_sum() {}

    double calculate(const std::vector <double> &input_vector) {
            double result = 0;
            for (int item = 0; item < input_vector.size(); ++item){
                result += input_vector[item];
            }
            if (input_vector.size() > 0) return result;
            else throw std::runtime_error("No input in function");
        }

    ~plugin_sum() {}
};

extern "C" BOOST_SYMBOL_EXPORT plugin_sum plugin;
plugin_sum plugin;
}