#ifndef DAFELAB_MANAGER_H
#define DAFELAB_MANAGER_H


#include <QList>
#include <QGlobalStatic>

#include "variable_data.h"
#include "instrument.h"
#include "utils.hpp"

#include "../models/datamodel.h"
#include "../models/instrumentmodel.h"
#include "../models/namingmodel.h"
#include "../models/visualmodel.h"

#include "../plots/plotscatter.h"

class DataModel;
class InstrumentModel;
class NamingModel;
class VisualModel;

class Manager {
public:
    Manager() = default;

    QList<VariableData> calculated;
    QList<VariableData> variables;

    VariableData &GetVariable(const QString &name);
    RawData GetRawData(const QString &name);

    void AddVariable(const VariableData &variable);

    void AddMeasurement();
    void DeleteMeasurement(size_t index);

    void DeleteVariable(size_t index);
    void DeleteCalculated(size_t index);

    void DeleteVariable(QString &name);
    void DeleteCalculated(QString &name);

    void ClearCalculated() { calculated.clear(); };
    void Clear();

    int GetMeasurementsCount();
    int GetVariablesCount() const;

    DataModel *data_model{nullptr};
    InstrumentModel *instrument_model{nullptr};
    NamingModel *naming_model{nullptr};
    VisualModel *visual_model{nullptr};
    QTableView *visual_table{nullptr};

    Plot* plot{nullptr};
    QCustomPlot* plot_field = nullptr;

    static Manager *instance();
};


#endif //DAFELAB_MANAGER_H
