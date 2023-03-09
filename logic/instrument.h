#ifndef INSTRUMENT_H
#define INSTRUMENT_H

enum class ErrorType {
    Absolute, Relative
};

class Instrument {
public:
    Instrument(ErrorType errorType, double value);

    ErrorType errorType() const;

    double value() const;

    void changeValue(double newValue);

private:
    ErrorType error_type_;
    double value_;
};

#endif  // INSTRUMENT_H