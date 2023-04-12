#include "instrument.h"

ErrorValue::ErrorValue(ErrorValue &error_value) : _type{error_value._type} {
    if (error_value._type == Single)
        value = error_value.value;
    else if (error_value._type == Multiple)
        list = error_value.list;
};

ErrorValue &ErrorValue::operator=(const ErrorValue &error_value) {
    _type = error_value._type;
    if (_type == Single)
        value = error_value.value;
    else if (_type == Multiple)
        list = error_value.list;
    return *this;
}

Instrument::Instrument(ErrorType type, double value)
        : type{type}, error{value} {};

Instrument::Instrument(ErrorType type, QList<double> const &list)
        : type{type}, error{list} {};
