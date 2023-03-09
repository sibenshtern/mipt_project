#ifndef VARIABLEDATA_H
#define VARIABLEDATA_H

#include <QList>
#include <QString>
#include "instrument.h"

class VariableData {
public:
    VariableData(QString fullNaming, QString shortNaming, Instrument instrument);

    QList<double> &measurements();

    QList<double> &error();

    QString fullNaming() const;

    QString shortNaming() const;

    Instrument instrument() const;

    double error(double measurement) const;

private:
    QList<double> m_measurements;
    QList<double> m_error;
    QString m_fullNaming;
    QString m_shortNaming;
    Instrument m_instrument;
};

#endif // VARIABLEDATA_H
