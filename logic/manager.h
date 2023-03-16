#pragma once

#include <QList>
#include <QModelIndex>  // temporary solution TODO: remove QModelIndex include

#include "variable_data.h"
#include "../models/measurement_model.h"


class MeasurementModel;

class Manager {
public:
    Manager() = default;
    Manager(QList<VariableData> &variables, QList<VariableData> &calculated);

    void AddVariable();

    template<typename T>
    void AddVariable(T&& variable);

    VariableData &GetVariable(int index);
    VariableData &GetVariable(QString &name);

    void DeleteVariable(int index);
    void DeleteVariable(QString &name);

    void AddMeasurementRow();
    void RemoveMeasurementRow(int index);

    void ClearCalculated() { calculated.clear(); };
    void AddCalculated(VariableData &variable);

    int GetMeasurementsCount() const { return _max_measurements_count; }
    int GetVariablesCount() const { return variables.size() + calculated.size(); }
private:
    QList<VariableData> variables;
    QList<VariableData> calculated;

    MeasurementModel *measurement_model {nullptr};

    int _max_measurements_count {0};
};

template<typename T>
void Manager::AddVariable(T&& variable) {
    variables.push_back(variable);
    _max_measurements_count = std::max(_max_measurements_count, variable.GetMeasurements().size());
    // TODO: rewrite this with interaction with Models
}
