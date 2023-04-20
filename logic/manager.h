#ifndef DAFELAB_MANAGER_H
#define DAFELAB_MANAGER_H


#include <QList>
#include <QGlobalStatic>

#include "variable_data.h"
#include "../models/datamodel.h"

class DataModel;

class Manager {
public:
    Manager() = default;

    QList<VariableData> calculated;
    QList<VariableData> variables;

    VariableData &GetVariable(QString &);

    void AddVariable(VariableData &);
    void AddVariable();
    void AddMeasurement();

    void DeleteVariable(int);
    void DeleteCalculated(int);

    void DeleteVariable(QString &);
    void DeleteCalculated(QString &);

    void ClearCalculated() { calculated.clear(); };

    int GetMeasurementsCount();
    int GetVariablesCount() const;

    DataModel *data_model{nullptr};

    static Manager *instance();
private:
    int max_variables_measurements{0};

    int max_calculated_measurements{0};
};


#endif //DAFELAB_MANAGER_H
