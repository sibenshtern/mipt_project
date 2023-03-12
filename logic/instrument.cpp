#include "instrument.h"

Instrument::Instrument(ErrorType type, double value)
        : type{type}, error{value} {}

Instrument::Instrument(ErrorType type, QList<double> list)
        : type{type}, error{list} {}

void Instrument::ChangeValue(double new_value) {
    if (new_value < 0)
        throw std::range_error("Error value must be greater than zero");
    if (type == ErrorType::Calculated)
        throw std::logic_error("Can't change single value in calculated error");

    error.value = new_value;
}

void Instrument::ChangeValue(QList<double> list) {
    for (size_t i = 0; i < list.size(); ++i)
        if (list[(int) i] < 0)
            throw std::range_error("Error value must be greater than zero");
    if (type != ErrorType::Calculated)
        throw std::logic_error("Can't change list of error in single value error");

    error.list = list;
}