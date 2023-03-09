#include "instrument.h"

Instrument::Instrument(ErrorType errorType, double value) :
        error_type_(errorType),
        value_(value)
{}

ErrorType Instrument::errorType() const {
    return error_type_;
}

double Instrument::value() const {
    return value_;
}

void Instrument::changeValue(double newValue) {
    value_ = newValue;
}
