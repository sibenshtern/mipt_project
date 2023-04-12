#include "manager.h"

VariableData &Manager::GetVariable(QString &name) {
    for (auto &variable: variables)
        if (variable.full_name == name)
            return variable;

    for (auto &calculate : calculated)
        if (calculate.full_name == name)
            return calculate;

    throw std::invalid_argument("No (calculated) variable with name " + name.toStdString());
}

void Manager::AddVariable(VariableData &data) {
    int measurement_count = GetMeasurementsCount();

    variables.append(data);

    int rows_count = std::max(0, data.measurements.size() - measurement_count);
    for (int i = 0; i < rows_count; ++i) {
        measurement_model->insertRow(measurement_count + i);
    }

    measurement_model->insertColumn(GetVariablesCount());

    measurement_count = GetMeasurementsCount();
    for (auto &variable : variables) {
        for (int j = 0; j < std::max(0, measurement_count - variable.measurements.size()); ++j)
            variable.measurements.push_back(0);
    }
}

void Manager::AddMeasurement() {
    for (auto &variable : variables) {
        variable.measurements.push_back(0);
    }
}

void Manager::DeleteVariable(int index) {
    if (index < 0 || index >= variables.size())
        throw std::range_error("Wrong index");

    variables.erase(variables.begin() + index);
}

void Manager::DeleteCalculated(int index) {
    if (index < 0 || index > calculated.size())
        throw std::invalid_argument("Wrong index");

    calculated.erase(calculated.begin() + index);
}

void Manager::DeleteVariable(QString &full_name) {
    for (int i = 0; i < variables.size(); ++i)
        if (variables[i].full_name == full_name) {
            variables.erase(variables.begin() + i);
            return;
        }

    throw std::invalid_argument("No variable with name " + full_name.toStdString());
}

void Manager::DeleteCalculated(QString &full_name) {
    for (int i = 0; i < calculated.size(); ++i)
        if (calculated[i].full_name == full_name) {
            calculated.erase(calculated.begin() + i);
            return;
        }

    throw std::invalid_argument("No calculated variable with name " + full_name.toStdString());
}

int Manager::GetMeasurementsCount() {
    for (auto &variable : variables)
        if (max_variables_measurements < variable.measurements.size())
            max_variables_measurements = variable.measurements.size();

    for (auto &calculate : calculated) {
        if (max_calculated_measurements < calculate.measurements.size())
            max_calculated_measurements = calculate.measurements.size();
    }

    return std::max(max_variables_measurements, max_calculated_measurements);
}

int Manager::GetVariablesCount() const {
    return variables.size() + calculated.size();
}

Q_GLOBAL_STATIC(Manager, manager);

Manager *Manager::instance() {
    return manager;
}

