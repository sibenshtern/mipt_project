#include "io.h"

void IOCSV::load(QString file_name) {
    auto manager = Manager::instance();

    QFile csv_file(file_name);

    if (!csv_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    boost::escaped_list_separator separator{'\\', ';', '\n'};

    QTextStream in(&csv_file);

    manager->Clear();

    QList<VariableData> variables;
    QList<QString> variables_names;

    std::string variables_name = in.readLine().toStdString();
    std::vector<std::string> tokens;
    Tokenizer tokenizer(variables_name, separator);
    tokens.assign(tokenizer.begin(), tokenizer.end());

    std::vector<std::string> error_parameters;
    std::string error_type;
    std::string instrument_name;
    double error_value = -1;

    for (const auto &token: tokens) {
        if (token.empty()) continue;
        size_t details_index_start = token.find('[');
        size_t details_index_end = token.find(']');

        std::string variable_name = token.substr(0, token.find('['));

        if (variables_names.contains(QString{variable_name.c_str()}))
            return;
        else
            variables_names.append(QString{variable_name.c_str()});

        std::string error_data = token.substr(details_index_start + 1, details_index_end - 2);
        qDebug() << "IOCSV::load(error_data)" << error_data.c_str();
        if (!error_data.empty() && error_data.find(',') != std::string::npos)
            boost::split(error_parameters, error_data, boost::is_any_of(","));

        if (error_parameters.size() >= 2) {
            error_type = error_parameters[0];
            size_t point = error_parameters[1].find('.');
            if (point != std::string::npos)
                error_parameters[1] = error_parameters[1].replace(point, point, ",");
            error_value = std::stod(error_parameters[1]);
            qDebug() << "IOCSV::load(error_value): " << error_value;
            if (error_parameters.size() == 3)
                instrument_name = error_parameters[2];
            else
                instrument_name = "";
        }

        Instrument instrument;

        if (error_type == "Absolute") {
            instrument.error.type = ErrorType::Absolute;
        } else if (error_type == "Relative") {
            instrument.error.type = ErrorType::Relative;
        } else {
            qDebug() << "Unknown error type. By default set Absolute";
            instrument.error.type = ErrorType::Absolute;
        }

        if (error_value >= 0) {
            instrument.error.value = error_value;
        } else if (error_value < 0) {
            qDebug() << "Error value invalid. By default set 0";
            instrument.error.value = 0;
        }

        if (!instrument_name.empty())
            instrument.name = QString{instrument_name.c_str()};
        else {
            qDebug() << "Instrument name empty. By default set \"Default instrument\"";
        }

        variables.append(VariableData{QString{variable_name.c_str()}, QString{variable_name.c_str()}, instrument});
    }

    while (!in.atEnd()) {
        std::string line = in.readLine().toStdString();
        Tokenizer line_tokenizer(line, separator);
        tokens.assign(line_tokenizer.begin(), line_tokenizer.end());

        for (int i = 0; i < tokens.size(); ++i) {
            if (tokens[i].empty())
                continue;
            size_t point = tokens[i].find('.');
            if (point != std::string::npos)
                tokens[i] = tokens[i].replace(point, point, ",");
            qDebug() << "IOCSV::load(tokens[i]): " << tokens[i].c_str();
            variables[i].measurements.push_back(std::stod(tokens[i]));
        }
    }

    qDebug() << "IOCSV::load(variables.size()): " << variables.size();
    for (auto &variable: variables) {
        Manager::instance()->AddVariable(variable);
    }
}

QString IOCSV::save() {
    QString result;

    for (const auto &variable: Manager::instance()->variables) {
        result += variable.naming.alias + "[";
        switch (variable.instrument.error.type) {
            case ErrorType::Absolute:
                result += "Absolute," + QString::number(variable.instrument.error.value) + ",";
                break;
            case ErrorType::Relative:
                result += "Relative," + QString::number(variable.instrument.error.value) + ",";
                break;
            case ErrorType::Calculated:
                result += "Calculated,,";
                break;
        }
        result += variable.instrument.name + "];";
    }
    result += "\n";

    for (int i = 0; i < Manager::instance()->GetMeasurementsCount(); ++i) {
        for (const auto &variable: Manager::instance()->variables)
            result += QString::number(variable.measurements[i]) + ";";
        result += "\n";
    }

    return result;
}
