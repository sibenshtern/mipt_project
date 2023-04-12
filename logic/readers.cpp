#include "readers.h"

// TODO: remove std::cout
void StrategyIOCSV::load() {
    QString file_name = QFileDialog::getOpenFileName(nullptr,
            "Open File", "$HOME", "CSV File (*csv)");
    QFile csv(file_name);

    if (!csv.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    std::vector<std::string> tokens;
    boost::escaped_list_separator<char> separator{'\\', ';', '\n'};
    QTextStream in(&csv);

    QList<VariableData> variables;
    int variables_count = 0;
    while (!in.atEnd()) {
        std::string line = in.readLine().toStdString();
        Tokenizer tokenizer(line, separator);
        tokens.assign(tokenizer.begin(), tokenizer.end());

        if (std::all_of(tokens[0].begin(), tokens[0].end(), [](char c) { return std::isalpha(c); })) {
            for (const auto & token : tokens) {
                QString name{token.data()};
                variables.append(VariableData{name, name, Instrument{}});
            }
            variables_count = (int)tokens.size();
        } else {
            std::cout << "Variable count: " << variables_count << " - ";
            std::cout << tokens.size() << ": ";
            for (int i = 0; i < tokens.size(); i++) {
                std::cout << QString{tokens[i].c_str()}.toDouble() << " ";
                variables[i].measurements.push_back(QString{tokens[i].c_str()}.toDouble());
            }
            std::cout << '\n';
        }
    }

    for (auto& variable : variables) {
        Manager::instance()->AddVariable(variable);
    }
}

void StrategyIOCSV::save() {

};
