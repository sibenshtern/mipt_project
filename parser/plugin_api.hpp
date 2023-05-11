#pragma once

#include <boost/config.hpp>
#include "Node.hpp"

class BOOST_SYMBOL_VISIBLE plugin_api {

public:
    virtual Node calculate(Node input_node) = 0;

    virtual ~plugin_api() {}
};