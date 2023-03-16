#pragma once

#include <QList>
#include <QString>
#include <stdexcept>
#include "instrument.h"

class VariableData {
public:
    VariableData() = default;
    VariableData(QString full_name, QString short_name, Instrument &instrument);
    template<typename T, typename P>
    VariableData(QString full_name, QString short_name, T&& instrument,
                 P&& measurements);
    VariableData(const VariableData &);

    void ChangeFullName(QString &);
    void ChangeShortName(QString &);
    void ChangeInstrument(Instrument &);
    void ChangeMeasurement(int, double);
    void ChangeMeasurements(QList<double> &);

    void AddMeasurement(double measurement);
    void DeleteMeasurement(int index);

    QString GetFullName() { return full_name; }
    QString GetShortName() { return short_name; }
    QList<double> GetMeasurements() const { return measurements; }
    double GetMeasurement(int index) const;
private:
    QList<double> measurements;
    QString full_name;
    QString short_name;
    Instrument instrument;
};

template<typename T, typename P>
VariableData::VariableData(QString full_name, QString short_name, T&& instrument, P&& measurements)
        : VariableData(full_name, short_name, instrument) {
    this->measurements = measurements;
};
