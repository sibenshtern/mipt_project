#include <boost/config.hpp>
#include <string>
#include <vector>

class BOOST_SYMBOL_VISIBLE plugin_api {

public:
    virtual double calculate(const std::vector <double> &input_vector) = 0;

    virtual ~plugin_api() {}
};