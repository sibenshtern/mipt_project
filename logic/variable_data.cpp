#include "variable_data.h"

double VariableData::Error(int index) {
    if (index < 0 || index >= measurements.size())
        throw std::range_error("Wrong index.");

    switch (instrument.type) {
        case (ErrorType::Absolute):
            return instrument.error.value;
        case (ErrorType::Relative):
            return instrument.error.value * measurements[index];
        case (ErrorType::Calculated):
            return instrument.error.list[index];
    }
}

double &VariableData::operator[](int index) {
    if (index < 0 || index >= measurements.size())
        throw std::range_error("Wrong index.");

    return measurements[index];
}

VariableData::VariableData(const VariableData &data) {
    full_name = data.full_name;
    short_name = data.short_name;
    instrument = data.instrument;
    measurements = data.measurements;
}
