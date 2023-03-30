#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
#include "plugin_api.hpp"
#include <iostream>


namespace plugin_namespace {

    class plugin_arithmetic_average: public plugin_api {
    public:
        plugin_arithmetic_average() {}

        double calculate(const std::vector <double> &input_vector) {
            double result = 0;
            for (int item = 0; item < input_vector.size(); ++item){
                result += input_vector[item];
            }
            return result;
        }

        ~plugin_arithmetic_average() {}
    };

    extern "C" BOOST_SYMBOL_EXPORT plugin_arithmetic_average plugin;
    plugin_arithmetic_average plugin;

}