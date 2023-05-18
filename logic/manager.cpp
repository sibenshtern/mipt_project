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

void Manager::AddVariable(const VariableData &variable) {
    int measurement_count = GetMeasurementsCount();

    if (variable.naming.full == "" && variable.naming.alias == "")
        const_cast<VariableData &>(variable).SetDefaultName(variables.size() + 1);
    variables.push_back(variable);

    int rows_count = std::max(0, variable.measurements.size() - measurement_count);
    for (int i = 0; i < rows_count; ++i)
        data_model->insertRow(measurement_count + i);

    data_model->insertColumn(GetVariablesCount());
    instrument_model->insertRow(GetVariablesCount());
    naming_model->insertRow(GetVariablesCount());
    visual_model->insertRow(GetVariablesCount());

    for (auto& _variable : variables) {
        int variable_size = _variable.measurements.size();
        for (int i = 0; i < GetMeasurementsCount() - variable_size; ++i)
            _variable.measurements.push_back(0);
    }
    plot->draw(plot_field);
}

void Manager::AddMeasurement() {
    for (auto &variable : variables) {
        variable.measurements.push_back(0);
    }
}

void Manager::DeleteMeasurement(size_t index) {
    if (index >= GetMeasurementsCount())
        throw std::range_error("Index bigger that measurements count.");

    for (auto &variable: variables)
        variable.measurements.erase(variable.measurements.begin() + static_cast<int>(index));
    for (auto &calculate: calculated)
        calculate.measurements.erase(calculate.measurements.begin() + static_cast<int>(index));

    data_model->removeRow(static_cast<int>(index));
    if (GetMeasurementsCount() == 0) {
        data_model->removeColumns(0, GetVariablesCount());
        instrument_model->removeRows(0, GetVariablesCount());
        naming_model->removeRows(0, GetMeasurementsCount());
        Clear();
    }
}

void Manager::DeleteVariable(size_t index) {
    if (index >= variables.size())
        throw std::range_error("Wrong index");

    int measurement_count = variables[static_cast<int>(index)].measurements.size();

    variables.erase(variables.begin() + static_cast<int>(index));
    data_model->removeColumn(static_cast<int>(index));
    instrument_model->removeRow(static_cast<int>(index));
    naming_model->removeRow(static_cast<int>(index));

    qDebug() << "Manager::DeleteVariable(index): " << index;

    if (variables.empty())
        data_model->removeRows(0, measurement_count);
}

void Manager::DeleteCalculated(size_t index) {
    if (index > calculated.size())
        throw std::invalid_argument("Wrong index");

    calculated.erase(calculated.begin() + static_cast<int>(index));
}

void Manager::DeleteVariable(QString &name) {
    for (int i = 0; i < variables.size(); ++i)
        if (variables[i].naming.full == name || variables[i].naming.alias == name) {
            variables.erase(variables.begin() + i);
            data_model->removeColumn(i);
            return;
        }

    throw std::invalid_argument("No variable with name " + name.toStdString());
}

void Manager::DeleteCalculated(QString &name) {
    for (int i = 0; i < calculated.size(); ++i)
        if (calculated[i].naming.alias == name) {
            calculated.erase(calculated.begin() + i);
            return;
        }

    throw std::invalid_argument("No calculated variable with name " + name.toStdString());
}

int Manager::GetMeasurementsCount() {
    int measurements_count = 0;
    for (auto &variable : variables)
        measurements_count = std::max(measurements_count, variable.measurements.size());

    for (auto &calculate : calculated)
        measurements_count = std::max(measurements_count, calculate.measurements.size());

    return measurements_count;
}

int Manager::GetVariablesCount() const {
    return variables.size() + calculated.size();
}

void Manager::Clear() {
    data_model->removeRows(0, GetMeasurementsCount());
    data_model->removeColumns(0, GetVariablesCount());
    instrument_model->removeRows(0, GetVariablesCount());
    naming_model->removeRows(0, GetVariablesCount());

    variables.clear();
    calculated.clear();
}

Q_GLOBAL_STATIC(Manager, manager);

Manager *Manager::instance() {
    return manager;
}

