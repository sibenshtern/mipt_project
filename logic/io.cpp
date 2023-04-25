#include "io.h"

void IOCSV::load(QString file_name) {
    auto manager = Manager::instance();

    QFile csv(file_name);

    if (!csv.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    std::vector<std::string> tokens;
    boost::escaped_list_separator<char> separator{'\\', ';', '\n'};
    QTextStream in(&csv);

    for (int i = manager->GetVariablesCount() - 1; i >= 0; i--)
        manager->data_model->removeColumn(i);

    for (int i = manager->GetMeasurementsCount() - 1; i >= 0; i--)
        manager->data_model->removeRow(i);

    Manager::instance()->Clear();

    QList<VariableData> variables;
    while (!in.atEnd()) {
        std::string line = in.readLine().toStdString();
        Tokenizer tokenizer(line, separator);
        tokens.assign(tokenizer.begin(), tokenizer.end());

        if (std::all_of(tokens[0].begin(), tokens[0].end(), [](char c) { return std::isalpha(c); })) {
            for (const auto & token : tokens) {
                QString name{token.data()};
                variables.append(VariableData{name, name, Instrument{}});
            }
        } else
            for (int i = 0; i < tokens.size(); i++)
                variables[i].measurements.push_back(QString{tokens[i].c_str()}.toDouble());

    }

    for (auto& variable : variables) {
        Manager::instance()->AddVariable(variable);
    }
}

