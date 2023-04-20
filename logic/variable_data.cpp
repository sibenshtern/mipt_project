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
    naming.full = data.naming.full;
    naming.alias = data.naming.alias;
    instrument = data.instrument;
    measurements = data.measurements;
}

VariableData::VariableData(int n) {
    naming.full = "";
    naming.alias = "";
    instrument = Instrument{};
    measurements = QVector<double>(n, 0).toList();
}
