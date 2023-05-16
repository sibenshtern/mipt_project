#include "variable_data.h"

double VariableData::Error(size_t measurement_index) {
    if (measurement_index >= measurements.size())
        throw std::range_error("Wrong index.");

    switch (instrument.error.type) {
        case (ErrorType::Absolute):
            return instrument.error.value;
        case (ErrorType::Relative):
            return instrument.error.value * measurements[static_cast<int>(measurement_index)];
        case (ErrorType::Calculated):
            return instrument.error.list[static_cast<int>(measurement_index)];
    }
}

double &VariableData::operator[](size_t index) {
    if (index >= measurements.size())
        throw std::range_error("Wrong index.");

    return measurements[static_cast<int>(index)];
}

VariableData::VariableData(
        QString full_name, QString short_name,
        const Instrument &instrument, const QList<double> &measurements)
        : VariableData(std::move(full_name), std::move(short_name), instrument) {
    this->measurements = measurements;
}

void VariableData::SetDefaultName(size_t variable_index) {
    QString name = QString{("variable " + std::to_string(variable_index)).c_str()};
    naming.full = name;
    naming.alias = name;
}

VariableData::VariableData(const VariableData &data) {
    naming.full = data.naming.full;
    naming.alias = data.naming.alias;
    instrument = data.instrument;
    measurements = data.measurements;
}

VariableData::VariableData(int measurements_count) {
    naming.full = "";
    naming.alias = "";
    instrument = Instrument{};
    measurements = QVector<double>(measurements_count, 0).toList();
}
