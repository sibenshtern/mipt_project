#ifndef DAFELAB_VARIABLE_DATA_H
#define DAFELAB_VARIABLE_DATA_H

#include <QString>
#include <QColor>
#include <utility>
#include <stdexcept>

#include "instrument.h"


struct Visual {
    int width = 5;
    bool visible = true;
    QString line_type = "solid";
    QString point_type = "circle";
    QColor color = "green";
};

class VariableData {
public:
    VariableData() = default;
    VariableData(int n);

    template<typename I>
    VariableData(QString, QString, I&&);

    template<typename I, typename M>
    VariableData(QString, QString, I&&, M&&);

    VariableData(const VariableData &);

    double Error(int);

    double &operator[](int);

    QList<double> measurements;
    struct {
        QString full;
        QString alias;
    } naming;
    Instrument instrument;
    Visual visual;
};

template<typename I>
VariableData::VariableData(QString full_name, QString short_name, I&& instrument)
        : naming{std::move(full_name), std::move(short_name)}, instrument{instrument} {}

template<typename I, typename M>
VariableData::VariableData(QString full_name, QString short_name, I&& instrument, M&& measurements)
        : VariableData(full_name, short_name, instrument) {
            this->measurements = measurements;
        }

#endif //DAFELAB_VARIABLE_DATA_H
