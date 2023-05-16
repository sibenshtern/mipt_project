#ifndef DAFELAB_IO_H
#define DAFELAB_IO_H

#include <vector>
#include <string>
#include <iterator>
#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>

#include "../logic/manager.h"

class IO {
public:
    virtual void load(QString file_name) = 0;
    virtual QString save() = 0;
};

class IOCSV : public IO {
public:
    void load(QString file_name) override;
    QString save() override;
private:
    typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
};

class IOJSON {
public:
    void load(QString file_name);
    QJsonDocument save();
};

#endif //DAFELAB_IO_H
