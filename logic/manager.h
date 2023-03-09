#ifndef MANAGER_H
#define MANAGER_H

#include <QList>
#include "variable_data.h"

class Manager {
public:
    void addVariable(VariableData variable);

    void deleteVariable(int index);

    void addMeasurementRow(int variableIndex, double measurement);

    void removeMeasurementRow(int variableIndex, int measurementIndex);

    void clearCalculated();

    void addCalculated(VariableData variable);

    QList<VariableData> variables() const;

    QList<VariableData> calculated() const;

private:
    QList<VariableData> m_variables;
    QList<VariableData> m_calculated;
};

#endif // MANAGER_H
