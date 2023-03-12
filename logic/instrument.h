#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QList>
#include <stdexcept>

enum class ErrorType {
    Absolute, Relative, Calculated
};

struct ErrorValue {
    explicit ErrorValue(QList<double> &list) : list{list}, _type{Multiple} {};
    explicit ErrorValue(double value) : value{value}, _type{Single} {};
    ErrorValue(ErrorValue &);
    ErrorValue &operator=(const ErrorValue &);

    ~ErrorValue() { list.~QList(); }
    union {
        QList<double> list;
        double value;
    };
    enum {Single, Multiple} _type;
};

class Instrument {
public:
    Instrument(ErrorType, double);
    Instrument(ErrorType, QList<double>);
    Instrument(Instrument &) = default;
    Instrument &operator=(const Instrument &) = default;

    ErrorType &GetType() { return type; }
    ErrorValue &GetError() { return error; }

    void ChangeValue(double);
    void ChangeValue(QList<double>);
private:
    ErrorType type;
    ErrorValue error;
};

#endif  // INSTRUMENT_H