#ifndef DAFELAB_INSTRUMENT_H
#define DAFELAB_INSTRUMENT_H

#include <QList>
#include <QDebug>

enum class ErrorType {
    Absolute, Relative, Calculated
};

struct Error {
    Error() : Error(0, ErrorType::Absolute) {}
    Error(double value, ErrorType error_type);
    explicit Error(const QList<double> &error_list, ErrorType error_type = ErrorType::Calculated);
    Error(Error &error);
    Error &operator=(const Error &error);

    ~Error() { if(type == ErrorType::Calculated) list.~QList(); }
    
    QList<double> list;
    double value;

    ErrorType type;
};

class Instrument {
public:
    Instrument() : Instrument(ErrorType::Absolute, 0, "Default instrument") {};
    Instrument(ErrorType error_type, double error_value, QString name = "Default");
    explicit Instrument(QList<double> const &error_list, QString name = "Default");
    Instrument(const Instrument &instrument);
    Instrument &operator=(const Instrument &instrument) = default;

    Error error;
    QString name;
};


#endif //DAFELAB_INSTRUMENT_H
