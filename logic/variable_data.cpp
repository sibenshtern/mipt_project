#include "variable_data.h"

VariableData::VariableData(QString &fullName, QString &shortName, Instrument &instrument)
        : full_name{fullName}, short_name{shortName}, instrument{instrument} {};

VariableData::VariableData(QString &full_name, QString &short_name, Instrument &instrument, QList<double> &measurements)
        : VariableData(full_name, short_name, instrument) {
    this->measurements = measurements;
};

VariableData::VariableData(const VariableData &data) {
    full_name = data.full_name;
    short_name = data.short_name;
    instrument = data.instrument;
    measurements = data.measurements;
}

void VariableData::ChangeFullName(QString &fullName) {
    if (fullName.isEmpty())
        throw std::invalid_argument("full name cannot be empty");
    full_name = fullName;
}

void VariableData::ChangeShortName(QString &shortName) {
    if (shortName.isEmpty())
        throw std::invalid_argument("short name cannot be empty");
    short_name = shortName;
}

void VariableData::ChangeInstrument(Instrument &inst) {
    instrument = inst;
}

void VariableData::ChangeMeasurement(int measurement_index, double measurement) {
    if (measurement_index < 0 || measurement_index > measurements.size())
        throw std::range_error("measurement index must be greater than zero and lower than measurements count");
    measurements[measurement_index] = measurement;
}

void VariableData::ChangeMeasurements(QList<double> &new_measurements) {
    measurements = new_measurements;
}