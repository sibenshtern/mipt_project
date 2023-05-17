#ifndef DAFELAB_VARIABLE_DATA_H
#define DAFELAB_VARIABLE_DATA_H

#include <QString>
#include <QColor>
#include <utility>
#include <stdexcept>
#include <QMap>
#include "../qcustomplot.h"

#include "instrument.h"

struct Visual {
    int width {5};
    bool visible {true};
    bool error_bars {true};
    QColor color {"red"};
    QString line_type {"Solid"};
    QString point_type {"None"};
    QMap<QString, Qt::PenStyle> line_types = {
            {"Solid", Qt::SolidLine},
            {"Dashed", Qt::DashLine},
            {"Dotted", Qt::DotLine}
    };
    QMap<QString, QCPScatterStyle> point_types = {
            {"None", QCPScatterStyle::ssNone},
            {"Cross", QCPScatterStyle::ssCross},
            {"Circle", QCPScatterStyle::ssCircle}
    };
};

class VariableData {
public:
    VariableData() = default;
    explicit VariableData(int measurements_count);

    VariableData(QString full_name, QString short_name, const Instrument &instrument)
        : naming{std::move(full_name), std::move(short_name)}, instrument{instrument} {};

    VariableData(
            QString full_name, QString short_name,
            const Instrument &instrument, const QList<double> &measurements);

    VariableData(const VariableData &);

    double Error(size_t measurement_index);

    double &operator[](size_t index);

    void SetDefaultName(size_t variable_index);

    QList<double> measurements;
    struct {
        QString full;
        QString alias;
    } naming;
    Instrument instrument;
    Visual visual;
};

#endif //DAFELAB_VARIABLE_DATA_H
