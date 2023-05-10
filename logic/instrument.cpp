#include "instrument.h"

#include <utility>

ErrorValue::ErrorValue(ErrorValue &error_value) : _type{error_value._type} {
    if (error_value._type == ErrorValueType::Single)
        value = error_value.value;
    else if (error_value._type == ErrorValueType::Multiple)
        list = error_value.list;
};

ErrorValue &ErrorValue::operator=(const ErrorValue &error_value) {
    _type = error_value._type;
    if (_type == ErrorValueType::Single)
        value = error_value.value;
    else if (_type == ErrorValueType::Multiple)
        list = error_value.list;
    return *this;
}

Instrument::Instrument(ErrorType type, double value, QString name)
        : type{type}, error{value}, name{std::move(name)} {};

Instrument::Instrument(ErrorType type, QList<double> const &list, QString name)
        : type{type}, error{list}, name{std::move(name)} {};

Instrument::Instrument(const Instrument &data) {
    type = data.type;
    error = data.error;
    name = data.name;
}
