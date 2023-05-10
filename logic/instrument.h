#ifndef DAFELAB_INSTRUMENT_H
#define DAFELAB_INSTRUMENT_H


#include <QList>

enum class ErrorType {
    Absolute, Relative, Calculated
};

enum ErrorValueType {
    Single, Multiple
};

struct ErrorValue {
    explicit ErrorValue(QList<double> const &list) : list{list}, _type{ErrorValueType::Multiple} {};
    explicit ErrorValue(double value) : value{value}, _type{ErrorValueType::Single} {};
    ErrorValue(ErrorValue &);
    ErrorValue &operator=(const ErrorValue &);

    ~ErrorValue() { if (_type == ErrorValueType::Multiple) list.~QList(); }
        QList<double> list;
        double value;
    
    ErrorValueType _type;
};


class Instrument {
public:
    Instrument() : Instrument(ErrorType::Absolute, 0) {};
    Instrument(ErrorType, double);
    Instrument(ErrorType, QList<double> const &);
    Instrument(Instrument &) = default;
    Instrument &operator=(const Instrument &) = default;

    ErrorType type;
    ErrorValue error;
};


#endif //DAFELAB_INSTRUMENT_H
