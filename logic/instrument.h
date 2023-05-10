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
    ErrorValue() : ErrorValue(0) {};
    explicit ErrorValue(QList<double> const &list) : list{list}, _type{ErrorValueType::Multiple} {};
    explicit ErrorValue(double value) : value{value}, _type{ErrorValueType::Single} {};
    ErrorValue(ErrorValue &);
    ErrorValue &operator=(const ErrorValue &);

    ~ErrorValue() { if (_type == ErrorValueType::Multiple) list.~QList(); }
    [[maybe_unused]] QList<double> list;
    [[maybe_unused]] double value;
    
    ErrorValueType _type;
};


class Instrument {
public:
    Instrument() : Instrument(ErrorType::Absolute, 0) {};
    Instrument(ErrorType, double, QString name = "Default");
    Instrument(ErrorType, QList<double> const &, QString name = "Default");
    Instrument(const Instrument &);
    Instrument &operator=(const Instrument &) = default;

    ErrorType type;
    ErrorValue error;
    QString name;
};


#endif //DAFELAB_INSTRUMENT_H
