#include "variable_data.h"

VariableData::VariableData(QString fullName, QString shortName, Instrument &instrument)
        : full_name{std::move(fullName)}, short_name{std::move(shortName)}, instrument{instrument} {};

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
    if (measurement_index < 0 || measurement_index >= measurements.size())
        throw std::range_error("measurement index must be greater than zero and lower than measurements count");
    measurements[measurement_index] = measurement;
}

void VariableData::ChangeMeasurements(QList<double> &new_measurements) {
    measurements = new_measurements;
}

void VariableData::AddMeasurement(double measurement) {
    measurements.push_back(measurement);
    if (instrument.GetType() == ErrorType::Calculated)
        instrument.GetError().list.push_back(0);
}

void VariableData::DeleteMeasurement(int index) {
    if (index < 0 || index >= measurements.size())
        throw std::invalid_argument("Wrong index");
    measurements.erase(measurements.begin() + index);

    if (instrument.GetType() == ErrorType::Calculated) {
        auto error = instrument.GetError();
        error.list.erase(error.list.begin() + index);
    }
}

double VariableData::GetMeasurement(int index) const {
    if (index < 0 || index >= measurements.size())
        throw std::invalid_argument("Wrong index");
    return measurements[index];
}