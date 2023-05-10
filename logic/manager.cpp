#include "manager.h"

VariableData &Manager::GetVariable(const QString &name) {
    for (auto &variable: variables)
        if (variable.naming.full == name || variable.naming.alias == name)
            return variable;

    for (auto &calculate : calculated)
        if (calculate.naming.full == name || calculate.naming.alias == name)
            return calculate;

    throw std::invalid_argument("No (calculated) variable with name " + name.toStdString());
}

RawData Manager::GetRawData(const QString &name) {
    auto variable = GetVariable(name);
    std::vector<double> errors(variable.measurements.size());

    for (int i = 0; i < variable.measurements.size(); ++i)
        errors[i] = variable.Error(i);

    return RawData{{variable.measurements.begin(), variable.measurements.end()}, errors};
}

void Manager::AddVariable(const VariableData &data) {
    int measurement_count = GetMeasurementsCount();

    variables.push_back(data);

    int rows_count = std::max(0, data.measurements.size() - measurement_count);
    for (int i = 0; i < rows_count; ++i) {
        data_model->insertRow(measurement_count + i);
    }

    data_model->insertColumn(GetVariablesCount());

    for (auto& variable : variables) {
        int variable_size = variable.measurements.size();
        for (int i = 0; i < GetMeasurementsCount() - variable_size; ++i)
            variable.measurements.push_back(0);
    }
}

void Manager::AddInstrument(const Instrument &data) {
    int instruments_count = static_cast<int>(instruments.size());
    instruments.append(data);
    qInfo() << instruments_count;
    instrument_model->insertRow(instruments_count);
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
    data_model->removeRow(index);
}

void Manager::DeleteCalculated(int index) {
    if (index < 0 || index > calculated.size())
        throw std::invalid_argument("Wrong index");

    calculated.erase(calculated.begin() + index);
}

void Manager::DeleteVariable(QString &full_name) {
    for (int i = 0; i < variables.size(); ++i)
        if (variables[i].naming.full == full_name) {
            variables.erase(variables.begin() + i);
            data_model->removeRow(i);
            return;
        }

    throw std::invalid_argument("No variable with name " + full_name.toStdString());
}

void Manager::DeleteCalculated(QString &full_name) {
    for (int i = 0; i < calculated.size(); ++i)
        if (calculated[i].naming.alias == full_name) {
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

void Manager::Clear() {
    variables.clear();
    calculated.clear();
    max_variables_measurements = 0;
    max_calculated_measurements = 0;
}

Q_GLOBAL_STATIC(Manager, manager);

Manager *Manager::instance() {
    return manager;
}

