#ifndef DAFELAB_READERS_H
#define DAFELAB_READERS_H

#include <vector>
#include <string>
#include <iterator>
#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/algorithm.hpp>

#include "../logic/manager.h"

class StrategyIO {
public:
    virtual void load() = 0;
    virtual void save() = 0;
};

class StrategyIOCSV : public StrategyIO {
public:
    virtual void load() override;
    virtual void save() override;
private:
    typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
};


#endif //DAFELAB_READERS_H
