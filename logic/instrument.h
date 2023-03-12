#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QList>
#include <stdexcept>

enum class ErrorType {
    Absolute, Relative, Calculated
};

union ErrorValue {
    explicit ErrorValue(QList<double> &list) : list{list} {};
    explicit ErrorValue(double value) : value{value} {};

    ~ErrorValue() { list.~QList(); }

    QList<double> list;
    double value;
};

class Instrument {
public:
    Instrument(ErrorType, double);
    Instrument(ErrorType, QList<double>);

    void ChangeValue(double);
    void ChangeValue(QList<double>);
private:
    ErrorType type;
    ErrorValue error;
};

#endif  // INSTRUMENT_H