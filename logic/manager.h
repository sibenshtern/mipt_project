#ifndef DAFELAB_MANAGER_H
#define DAFELAB_MANAGER_H


#include <QList>
#include <QGlobalStatic>

#include "variable_data.h"
#include "instrument.h"
#include "utils.hpp"

#include "../models/datamodel.h"
#include "../models/instrumentmodel.h"

#include "../plots/plotscatter.h"

class DataModel;
class InstrumentModel;

class Manager {
public:
    Manager() = default;

    QList<VariableData> calculated;
    QList<VariableData> variables;
    QList<Instrument> instruments;

    VariableData &GetVariable(const QString &);
    RawData GetRawData(const QString &);

    void AddVariable(const VariableData &);
    void AddInstrument(const Instrument &);
    void AddMeasurement();

    void DeleteVariable(int);
    void DeleteCalculated(int);

    void DeleteVariable(QString &);
    void DeleteCalculated(QString &);

    void ClearCalculated() { calculated.clear(); };
    void Clear();

    int GetMeasurementsCount();
    int GetVariablesCount() const;

    DataModel *data_model{nullptr};
    InstrumentModel *instrument_model{nullptr};

    Plot* plot{nullptr};
    QCustomPlot* plot_field = nullptr;

    static Manager *instance();
};


#endif //DAFELAB_MANAGER_H
