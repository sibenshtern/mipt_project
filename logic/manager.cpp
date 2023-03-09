#include "manager.h"

void Manager::addVariable(VariableData variable) {
    m_variables.append(variable);
}

void Manager::deleteVariable(int index) {
    if (index < 0 || index >= m_variables.size()) {
        return; // Invalid index.
    }
    m_variables.removeAt(index);
}

void Manager::addMeasurementRow(int variableIndex, double measurement) {
    if (variableIndex < 0 || variableIndex >= m_variables.size()) {
        return; // Invalid variable index.
    }

    VariableData &variable = m_variables[variableIndex];
    QList<double> &measurements = variable.measurements();
    QList<double> &error = variable.error();

    measurements.append(measurement);
    error.append(variable.error(measurement));
}

void Manager::removeMeasurementRow(int variableIndex, int measurementIndex) {
    if (variableIndex < 0 || variableIndex >= m_variables.size()) {
        return; // Invalid variable index.
    }

    VariableData &variable = m_variables[variableIndex];
    QList<double> &measurements = variable.measurements();
    QList<double> &error = variable.error();

    if (measurementIndex < 0 || measurementIndex >= measurements.size()) {
        return; // Invalid measurement index.
    }

    measurements.removeAt(measurementIndex);
    error.removeAt(measurementIndex);
}

void Manager::clearCalculated() {
    m_calculated.clear();
}

void Manager::addCalculated(VariableData variable) {
    m_calculated.append(variable);
}

QList<VariableData> Manager::variables() const {
    return m_variables;
}

QList<VariableData> Manager::calculated() const {
    return m_calculated;
}

