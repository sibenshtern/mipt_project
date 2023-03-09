#include "variable_data.h"

VariableData::VariableData(QString fullNaming, QString shortNaming, Instrument instrument)
        : m_fullNaming(fullNaming),
          m_shortNaming(shortNaming),
          m_instrument(instrument) {
}

QList<double> &VariableData::measurements() {
    return m_measurements;
}

QList<double> &VariableData::error() {
    return m_error;
}

QString VariableData::fullNaming() const {
    return m_fullNaming;
}

QString VariableData::shortNaming() const {
    return m_shortNaming;
}

Instrument VariableData::instrument() const {
    return m_instrument;
}

double VariableData::error(double measurement) const {
    double error = 0.0;
    switch (m_instrument.errorType()) {
        case ErrorType::Absolute:
            error = m_instrument.value();
            break;
        case ErrorType::Relative:
            error = measurement * m_instrument.value();
            break;
    }
    return error;
}
