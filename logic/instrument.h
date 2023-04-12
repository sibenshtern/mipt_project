#ifndef DAFELAB_INSTRUMENT_H
#define DAFELAB_INSTRUMENT_H


#include <QList>

enum class ErrorType {
    Absolute, Relative, Calculated
};

struct ErrorValue {
    explicit ErrorValue(QList<double> const &list) : list{list}, _type{Multiple} {};
    explicit ErrorValue(double value) : value{value}, _type{Single} {};
    ErrorValue(ErrorValue &);
    ErrorValue &operator=(const ErrorValue &);

    ~ErrorValue() { if (_type == Multiple) list.~QList(); }
    union {
        QList<double> list;
        double value;
    };
    enum {Single, Multiple} _type;
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
