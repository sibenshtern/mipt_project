#ifndef MANAGER_H
#define MANAGER_H

#include <QList>
#include <QModelIndex>  // temporary solution TODO: remove QModelIndex include

#include "variable_data.h"
#include "../models/measurementmodel.h"

class Manager {
public:
    Manager() = default;
    Manager(QList<VariableData> &variables, QList<VariableData> &calculated);

    void AddVariable();
    void AddVariable(VariableData &variable);

    VariableData &GetVariable(int index);
    VariableData &GetVariable(QString &name);

    void DeleteVariable(int index);
    void DeleteVariable(QString &name);

    void AddMeasurementRow();
    void RemoveMeasurementRow(int index);

    void ClearCalculated() { calculated.clear(); };
    void AddCalculated(VariableData &variable);
private:
    QList<VariableData> variables;
    QList<VariableData> calculated;

    MeasurementModel *measurement_model {nullptr};

    int _max_measurements_count {0};
};

#endif // MANAGER_H
