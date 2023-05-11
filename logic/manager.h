#ifndef DAFELAB_MANAGER_H
#define DAFELAB_MANAGER_H


#include <QList>
#include <QGlobalStatic>

#include "variable_data.h"
#include "../models/datamodel.h"
#include "../plots/plotscatter.h"
#include "utils.hpp"
#include "../logic/instrument.h"

class DataModel;

class Manager {
public:
    Manager() = default;

    QList<VariableData> calculated;
    QList<VariableData> variables;
    QList<Instrument> instruments;

    VariableData &GetVariable(const QString &);
    RawData GetRawData(const QString &);

    void AddVariable(VariableData &);
    void AddMeasurement();
    void AddInstrument(Instrument &);

    void DeleteVariable(int);
    void DeleteCalculated(int);

    void DeleteVariable(QString &);
    void DeleteCalculated(QString &);

    void ClearCalculated() { calculated.clear(); };
    void Clear();

    int GetMeasurementsCount();
    int GetVariablesCount() const;
    int GetInstrumentsCount();

    DataModel *data_model{nullptr};
    Plot* plot{nullptr};
    QCustomPlot* plot_field = nullptr;

    static Manager *instance();
private:
    int max_variables_measurements{0};

    int max_calculated_measurements{0};
};


#endif //DAFELAB_MANAGER_H
