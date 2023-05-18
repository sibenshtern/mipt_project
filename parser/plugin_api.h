#ifndef PLUGIN_API_H
#define PLUGIN_API_H

#include <boost/config.hpp>
#include "Node.h"

class BOOST_SYMBOL_VISIBLE plugin_api {

public:
    virtual Node calculate(Node input_node) = 0;

    virtual ~plugin_api() {}
};

#endif //PLUGIN_API_H