#include "instrument.h"

#include <utility>

Error::Error(double value, ErrorType error_type) : value{value} {
    if (error_type == ErrorType::Calculated)
        throw std::invalid_argument("Error type with number value can't be calculated");
    type = error_type;
}

Error::Error(const QList<double> &error_list, ErrorType error_type) {
    if (error_type != ErrorType::Calculated)
        throw std::invalid_argument("Error type with list value must be calculated");
    type = error_type;
    list = error_list;
}

Error::Error(Error &error) {
    if (error.type == ErrorType::Calculated)
        list = error.list;
    else
        value = error.value;
    type = error.type;
}

Error &Error::operator=(const Error &error) {
    type = error.type;
    if (type == ErrorType::Calculated)
        list = error.list;
    else
        value = error.value;
    return *this;
}

Instrument::Instrument(ErrorType error_type, double error_value, QString name) {
    error = Error{error_value, error_type};
    this->name = std::move(name);
}

Instrument::Instrument(QList<double> const &error_list, QString name) {
    error = Error{error_list, ErrorType::Calculated};
    this->name = std::move(name);
}

Instrument::Instrument(const Instrument &instrument) {
    error = instrument.error;
    name = instrument.name;
}
