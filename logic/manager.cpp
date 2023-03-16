#include "manager.h"

Manager::Manager(QList<VariableData> &variables, QList<VariableData> &calculated)
        : variables{variables}, calculated{calculated} {
    for (auto variable : variables) {
        int measurements_size = variable.GetMeasurements().size();
        if (measurements_size > _max_measurements_count)
            _max_measurements_count = measurements_size;
    }
};

void Manager::AddVariable() {
    variables.push_back(VariableData());
    // TODO: rewrite this with interaction with Models
}

VariableData &Manager::GetVariable(int index) {
    if (index < 0 || index > variables.size() + calculated.size())
        throw std::invalid_argument("Wrong index");
    else if (index < variables.size())
        return variables[index];
    else
        return calculated[index - variables.size()];
}

VariableData &Manager::GetVariable(QString &name) {
    for (int i = 0; i < variables.size(); ++i)
        if (variables[i].GetFullName() == name || variables[i].GetShortName() == name)
            return variables[i];

    for (int i = 0; i < calculated.size(); ++i)
        if (variables[i].GetFullName() == name || variables[i].GetShortName() == name)
            return calculated[i];

    throw std::invalid_argument("No variable/calculated with name " + name.toStdString());
}


void Manager::DeleteVariable(int index) {
    if (index < 0 || index >= variables.size())
        throw std::invalid_argument("Wrong index");
    variables.erase(variables.begin() + index);
}

void Manager::DeleteVariable(QString &name) {
    for (int i = 0; i < variables.size(); ++i)
        if (variables[i].GetFullName() == name || variables[i].GetShortName() == name) {
            variables.erase(variables.begin() + i);
            return;
        }
    throw std::invalid_argument("No variable with name: " + name.toStdString());
}

void Manager::AddMeasurementRow() {
    for (auto variable : variables)
        variable.AddMeasurement(0);
    // TODO: interaction with Models
}

void Manager::RemoveMeasurementRow(int index) {

    if (index < 0 || index >= _max_measurements_count)
        throw std::invalid_argument("Wrong index");

    for (auto variable : variables) {
        variable.DeleteMeasurement(index);
    }

    // TODO: interaction with Models
}

void Manager::AddCalculated(VariableData &variable) {
    calculated.push_back(variable);

    // TODO: interaction with Models
}