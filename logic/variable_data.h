#ifndef VARIABLEDATA_H
#define VARIABLEDATA_H

#include <QList>
#include <QString>
#include <stdexcept>
#include "instrument.h"

class VariableData {
public:
    VariableData() = default;
    VariableData(QString &full_name, QString &short_name, Instrument &instrument);
    VariableData(QString &full_name, QString &short_name, Instrument &instrument,
                 QList<double> &measurements);
    VariableData(const VariableData &);

    void ChangeFullName(QString &);
    void ChangeShortName(QString &);
    void ChangeInstrument(Instrument &);
    void ChangeMeasurement(int, double);
    void ChangeMeasurements(QList<double> &);

    QString GetFullName() { return full_name; }
    QString GetShortName() { return short_name; }

//    void error(int measurement);
private:
    QList<double> measurements;
    QString full_name;
    QString short_name;
    Instrument instrument;
};

#endif // VARIABLEDATA_H